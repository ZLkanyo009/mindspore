# Copyright 2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
"""
GPT-2 finetune and evaluation script for Children's Book Test task.
"""
import argparse
import time
import numpy as np

from mindspore import context
from mindspore.nn.wrap.loss_scale import DynamicLossScaleUpdateCell
from mindspore.nn import AdamWeightDecay, Lamb, Momentum
from mindspore.train.model import Model
from mindspore.train.callback import CheckpointConfig, ModelCheckpoint, TimeMonitor, LossMonitor
from mindspore.train.serialization import load_checkpoint, load_param_into_net

from src.gpt2_for_finetune import GPT2FinetuneCell, GPT2CBT
from src.finetune_eval_config import cfg, gpt2_net_cfg
from src.utils.metric_method import Accuracy
from src.dataset import create_cbt_dataset, create_language_model_dataset
from src.utils.lr_schedule import GPT2LearningRate
from src.utils.task_utils import calculate_choice_prob_for_cbt


def do_train(dataset=None, network=None, load_checkpoint_path="", save_checkpoint_path="", epoch_num=1):
    """
    Do train
    Args:
        dataset: the train dataset.
        network:  the network with loss
        load_checkpoint_path: the file path which saved pretrained model checkpoint.
        save_checkpoint_path:  the file path which will save finetuned model checkpoint.
        epoch_num: the number of epoch.
    """
    if load_checkpoint_path == "":
        raise ValueError("Pretrain model missed, finetune task must load pretrain model!")

    steps_per_epoch = dataset.get_dataset_size()

    # optimizer
    if cfg.optimizer == 'AdamWeightDecay':
        lr_schedule = GPT2LearningRate(learning_rate=cfg.AdamWeightDecay.learning_rate,
                                       end_learning_rate=cfg.AdamWeightDecay.end_learning_rate,
                                       warmup_steps=int(steps_per_epoch * epoch_num * 0.1),
                                       decay_steps=steps_per_epoch * epoch_num,
                                       power=cfg.AdamWeightDecay.power)
        params = network.trainable_params()

        decay_params = list(filter(cfg.AdamWeightDecay.decay_filter, params))
        other_params = list(filter(lambda x: not cfg.AdamWeightDecay.decay_filter(x), params))
        group_params = [{'params': decay_params, 'weight_decay': cfg.AdamWeightDecay.weight_decay},
                        {'params': other_params, 'weight_decay': 0.0}]
        optimizer = AdamWeightDecay(group_params, lr_schedule, eps=cfg.AdamWeightDecay.eps)
    elif cfg.optimizer == 'Lamb':
        lr_schedule = GPT2LearningRate(learning_rate=cfg.Lamb.learning_rate,
                                       end_learning_rate=cfg.Lamb.end_learning_rate,
                                       warmup_steps=int(steps_per_epoch * epoch_num * 0.1),
                                       decay_steps=steps_per_epoch * epoch_num,
                                       power=cfg.Lamb.power)
        optimizer = Lamb(network.trainable_params(), lr_schedule)
    elif cfg.optimizer == 'Momentum':
        optimizer = Momentum(network.trainable_params(), cfg.Momentum.learning_rate, cfg.Momentum.momentum)
    else:
        raise Exception("Optimizer not supported. support: [AdamWeightDecay, Lamb, Momentum]")

    # load checkpoint into network
    ckpt_config = CheckpointConfig(save_checkpoint_steps=steps_per_epoch, keep_checkpoint_max=1)
    prefix_name = "gpt2_" + "cbt_" + str(cfg.gpt2_network) + "_" + str(cfg.optimizer) + "_" + str(epoch_num) +\
                  "_bs" + str(gpt2_net_cfg.batch_size)
    ckpoint_cb = ModelCheckpoint(prefix=prefix_name,
                                 directory=None if save_checkpoint_path == "" else save_checkpoint_path,
                                 config=ckpt_config)
    param_dict = load_checkpoint(load_checkpoint_path)

    final_param_dict = {}
    for name, _ in param_dict.items():
        final_param_dict['gpt2.gpt2.' + name] = param_dict[name]
    final_param_dict['gpt2.lm_head.weight'] = param_dict['gpt2_embedding_lookup.embedding_table']

    load_param_into_net(network, final_param_dict)
    print("Load pretrained parameter successfully!\n")

    update_cell = DynamicLossScaleUpdateCell(loss_scale_value=2 ** 32, scale_factor=2, scale_window=1000)
    netwithgrads = GPT2FinetuneCell(network, optimizer=optimizer, scale_update_cell=update_cell)
    netwithgrads.set_train(True)

    loss_cb = LossMonitor(per_print_times=1)
    model = Model(netwithgrads)
    callbacks = [TimeMonitor(dataset.get_dataset_size()), loss_cb, ckpoint_cb]

    print("==================== Starting Finetuning ====================")
    model.train(epoch_num, dataset, callbacks=callbacks, dataset_sink_mode=False)
    print("==================== Finetuning Success  ====================")


def do_eval(dataset=None, network=None, metric=None, load_checkpoint_path="", eval_type=None, num_choice=None):
    """
    Do evaluation for CBT task.
    Args:
        dataset: the eval dataset.
        network:  the network with loss.
        metric: the evaluation method.
        load_checkpoint_path: the file path which saved finetuned model checkpoint.
        eval_type:
        num_choice:
    """
    if load_checkpoint_path == "":
        raise ValueError("Finetune model missed, evaluation task must load finetune model!")

    if metric.lower() == "accuracy":
        print("Prepare to calculate the accuracy score ...")
        gpt2_cbt = network(config=gpt2_net_cfg,
                           is_training=False,
                           use_one_hot_embeddings=False
                           )
        gpt2_cbt.set_train(False)
        param_dict = load_checkpoint(load_checkpoint_path)

        if eval_type == "zero-shot":
            final_param_dict = {}
            for name, _ in param_dict.items():
                final_param_dict['gpt2.gpt2.' + name] = param_dict[name]
            final_param_dict['gpt2.lm_head.weight'] = param_dict['gpt2_embedding_lookup.embedding_table']
            load_param_into_net(gpt2_cbt, final_param_dict)
            print("load pretrained parameter successfully!\n")
        elif eval_type == "finetuned":
            load_param_into_net(gpt2_cbt, param_dict)
            print("load finetuned parameter successfully!\n")
        else:
            raise ValueError("Evaluation type missed, eval_type should be [zero-shot, finetuned]")

        model = Model(gpt2_cbt)
        callback = Accuracy()
        columns_list = ["input_ids", "input_mask", "input_length", "mc_labels"]
        print("==================== [ACC] Testing ====================")
        num_data = 1
        all_choice_prob = []

        for data in dataset.create_dict_iterator():
            input_data = []
            for i in columns_list:
                input_data.append(data[i])
            input_ids, input_mask, input_length, mc_labels = input_data
            print("| [ACC] number : {} / {} ".format(num_data, dataset.get_dataset_size()))
            # print("mc_labels: {}".format(mc_labels))  # [batch_size]

            logits = model.predict(input_ids, input_mask)
            # choice_prob_list [batch_size]
            choice_prob_list = calculate_choice_prob_for_cbt(logits=logits,
                                                             batch_size=gpt2_net_cfg.batch_size,
                                                             input_length=input_length,
                                                             input_ids=input_ids)
            all_choice_prob.append(choice_prob_list)
            if (num_data * gpt2_net_cfg.batch_size) % num_choice == 0:
                all_choice_prob_np = np.array(all_choice_prob)
                all_choice_prob_np = all_choice_prob_np.reshape((-1, num_choice))
                print("| all_choice_prob_np: ", all_choice_prob_np)
                print("| all_choice_prob_np shape: ", all_choice_prob_np.shape)
                mc_labels = np.array([mc_labels.asnumpy()[0]])
                callback.update(all_choice_prob_np, mc_labels)
                all_choice_prob = []
            num_data += 1

        print("\n\n")
        print("**************************************************************")
        print("acc_num {} , total_num {}, accuracy {:.6f}".format(callback.acc_num, callback.total_num,
                                                                  callback.acc_num / callback.total_num))
        print("********************** Testing Finished **********************")
    else:
        raise ValueError("metric method not supported, support: [Accuracy]")


def run_cbt_task():
    """
    run Children's Book Test (CBT) task
    """
    parser = argparse.ArgumentParser(description="Finetune and Evaluate CBT task")
    parser.add_argument("--device_target", type=str, default="Ascend",
                        help="Device type. Default: Ascend.")
    parser.add_argument("--device_id", type=int, default=1,
                        help="ID of target device. ")
    parser.add_argument("--num_choice", type=int, default=10,
                        help="The number of choice in CBT task. ")
    parser.add_argument("--metric_method", type=str, default="Accuracy",
                        help="The eval method including [Accuracy]. Default: Accuracy.")
    parser.add_argument("--do_train", type=str, default="false",
                        help="Enable train. Default: false.")
    parser.add_argument("--do_eval", type=str, default="true",
                        help="Enable evaluation. Default: true.")
    parser.add_argument("--eval_type", type=str, default="zero-shot",
                        help="The type of evaluation including [zero-shot, finetuned]. Default: zero-shot.")
    parser.add_argument("--epoch_num", type=int, default=1,
                        help="Epoch number. Default: 1.")
    parser.add_argument("--train_data_shuffle", type=str, default="true",
                        help="Enable train data shuffle. Default: true.")
    parser.add_argument("--eval_data_shuffle", type=str, default="false",
                        help="Enable eval data shuffle. Default: false.")
    parser.add_argument("--save_finetune_ckpt_path", type=str, default="",
                        help="Save the finetuned checkpoint path.")
    parser.add_argument("--load_pretrain_ckpt_path", type=str, default="",
                        help="Load the checkpoint file path for train.")
    parser.add_argument("--load_finetune_ckpt_path", type=str, default="",
                        help="Load the checkpoint file path for evaluation.")
    parser.add_argument("--train_data_file_path", type=str, default="",
                        help="Data path, it is better to use absolute path")
    parser.add_argument("--eval_data_file_path", type=str, default="",
                        help="Data path, it is better to use absolute path")
    args_opt = parser.parse_args()

    epoch_num = args_opt.epoch_num
    metric = args_opt.metric_method
    save_finetune_ckpt_path = args_opt.save_finetune_ckpt_path
    load_finetune_ckpt_path = args_opt.load_finetune_ckpt_path
    load_pretrain_ckpt_path = args_opt.load_pretrain_ckpt_path

    if args_opt.do_train.lower() == "false" and args_opt.do_eval.lower() == "false":
        raise ValueError("At least one of 'do_train' or 'do_eval' must be true")
    if args_opt.do_train.lower() == "true" and args_opt.train_data_file_path == "":
        raise ValueError("'train_data_file_path' must be set when do finetune task")
    if args_opt.do_eval.lower() == "true" and args_opt.eval_data_file_path == "":
        raise ValueError("'eval_data_file_path' must be set when do evaluation task")

    device_target = args_opt.device_target
    if device_target == "Ascend":
        context.set_context(mode=context.GRAPH_MODE,
                            device_target=device_target,
                            device_id=args_opt.device_id,
                            max_call_depth=3000)
        context.set_auto_parallel_context(parallel_mode="stand_alone")
        print(" | Device: {}  | Device id: {}".format(device_target, args_opt.device_id))
    else:
        raise Exception("Device target error, Ascend is supported.")

    gpt2_loss = GPT2CBT(config=gpt2_net_cfg,
                        is_training=True,
                        use_one_hot_embeddings=False)

    if args_opt.do_train.lower() == "true":
        print("==============    Start Loading Train Dataset   ============")
        print(" | Train Dataset: {}".format(args_opt.train_data_file_path))
        print(" | Checkpoint: {}".format(args_opt.load_pretrain_ckpt_path))
        train_dataset = create_language_model_dataset(do_shuffle=(args_opt.train_data_shuffle.lower() == "true"),
                                                      dataset_path=args_opt.train_data_file_path)
        do_train(train_dataset, gpt2_loss, load_pretrain_ckpt_path, save_finetune_ckpt_path, epoch_num)

    if args_opt.do_eval.lower() == "true":
        print("============== Start Loading Evaluation Dataset ============")
        print(" | Eval Dataset: {}".format(args_opt.eval_data_file_path))
        print(" | Checkpoint: {}".format(args_opt.load_finetune_ckpt_path))
        eval_dataset = create_cbt_dataset(do_shuffle=(args_opt.eval_data_shuffle.lower() == "true"),
                                          dataset_path=args_opt.eval_data_file_path)
        do_eval(eval_dataset, GPT2CBT, metric, load_finetune_ckpt_path, args_opt.eval_type, args_opt.num_choice)


if __name__ == "__main__":
    print("Start Time: \n", time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    run_cbt_task()
    print("End Time: \n", time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
