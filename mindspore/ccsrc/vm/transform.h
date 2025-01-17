/**
 * This is the C++ adaptation and derivative work of Myia (https://github.com/mila-iqia/myia/).
 *
 * Copyright 2019 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MINDSPORE_CCSRC_VM_TRANSFORM_H_
#define MINDSPORE_CCSRC_VM_TRANSFORM_H_

#include <string>
#include <memory>
#include <functional>
#include <utility>
#include <unordered_map>
#include <vector>

#include "vm/vm.h"
#include "ir/anf.h"
#include "frontend/operator/ops.h"
#include "vm/segment_runner.h"
#include "vm/backend.h"
#include "vm/graph_partition.h"

// mindspore namespace is the top level namespace of MindSpore project.
// Other namespace should be a sub namespace of mindspore namespace in the ME project.
namespace mindspore {
extern const char kMsVm[];
extern const char kGeVm[];

// compile namespace
// A sub namespace in ME to support compile related definition.
namespace compile {
extern std::vector<PrimitivePtr> nonlinear_ops;
const std::vector<PrimitivePtr> &GetMsNonlinearOps();

using VmEvalFunc = std::function<BaseRef(const VectorRef &)>;
using VmEvalFuncPtr = std::shared_ptr<std::function<BaseRef(const VectorRef &)>>;

class CompileGraph {
 public:
  explicit CompileGraph(const BackendPtr &backend, const std::vector<PrimitivePtr> &cut_list = nonlinear_ops);

  ~CompileGraph() = default;

  InstSet Run(const FuncGraphPtr &func_graph);
  bool IsCut(const AnfNodePtr &node);
  void Push(const AnfNodePtr &node);
  void Tie(const AnfNodePtr &n1, const AnfNodePtr &n2) { slots_[n2] = slots_[n1]; }
  void Ret(int64_t nargs);
  int64_t Ref(const AnfNodePtr &node);

  void set_height(int64_t h) {
    height_ = h;
    if (height_ > max_height_) {
      max_height_ = height_;
    }
  }

  void Reset() {
    height_ = 0;
    max_height_ = 0;
    slots_.clear();
    inst_.clear();
  }

 private:
  void PushParameters(const FuncGraphPtr &func_graph);
  bool Compile(const FuncGraphPtr &func_graph);
  int64_t LinConvert(const FuncGraphPtr &func_graph, const GraphSegmentPtr &segment, const std::string &target = "");
  int64_t InterpretNode(const FuncGraphPtr &func_graph, const CNodePtr &node);
  int64_t AddCall(const FuncGraphPtr &graph, const CNodePtr &node);
  void AddPadStack(int64_t param_height);
  void AddTailCall(const AnfNodePtr &fn, size_t size);
  void AddPartial(const CNodePtr &node);
  void AddMakeTuple(const CNodePtr &node);
  void AddSwitch(const CNodePtr &node);
  void AddSwitchLayer(const CNodePtr &node);
  void AddReturn(const CNodePtr &node);
  void AddPrimitive(const CNodePtr &node, const PrimitivePtr &prim);
  void AddInput(const AnfNodePtr &node);
  void AddExternal(const LinConvertResult &result);
  void AddInst(const Instruction &inst, const int64_t &arg);
  void AddInst(const Instruction &inst, const ValuePtr &arg);
  void AddInst(const Instruction &inst, const VectorRef &args);

  BackendPtr backend_;
  GraphPartitionPtr graph_partition_;
  LinkFuncType lin_convert_;

  int64_t height_{0};
  int64_t max_height_{0};

  std::unordered_map<AnfNodePtr, int64_t> slots_;
  InstSet inst_;
};

using CompileGraphPtr = std::shared_ptr<CompileGraph>;

// CompileGraphs is used to Convert a graph cluster into instruction lists.
class CompileGraphs {
 public:
  explicit CompileGraphs(const BackendPtr &backend, const std::vector<PrimitivePtr> &cut_list = nonlinear_ops);

  ~CompileGraphs() = default;

  void Reset() {
    insts_.clear();
    mapping_.clear();
  }

  void Compile(const FuncGraphPtr &func_graph);
  FinalVMPtr Link(const FuncGraphPtr &func_graph);
  FinalVMPtr CompileAndLink(const FuncGraphPtr &func_graph);

 private:
  InstSet insts_;
  std::unordered_map<FuncGraphPtr, int64_t> mapping_;
  CompileGraphPtr transform_;
  BackendPtr backend_;
};

// The graph compiling of using mindRT, which transforms the funcGraph to kernelGraph and returns the graph id of
// kernelGraph.
class GraphCompiler {
 public:
  GraphCompiler(const std::shared_ptr<MindRTBackend> &backend,
                const std::vector<PrimitivePtr> &cut_list = nonlinear_ops);
  ~GraphCompiler() = default;

  // The parameter root_graph is a root graph, and the root graph maybe contain multiple sub graphs,
  // the return is the kernelGraph id of the root graph. It will traverse all subgraphs to call CompileGraph.
  uint32_t CompileGraphs(const FuncGraphPtr &root_graph);

 private:
  // The parameter func_graph is a graph, it can be either a root graph or a sub graph,
  // the return is the corresponding kernelGraph id of the graph.
  uint32_t CompileGraph(const FuncGraphPtr &func_graph);

  std::shared_ptr<MindRTBackend> backend_;
  GraphPartitionPtr graph_partition_;
};

// Judge whether to use mindRT. GPU and CPU use mindRT currently, and other hardwares will use it in the future.
bool IsMindRTUsed();

BackendPtr CreateBackend();

}  // namespace compile
}  // namespace mindspore

#endif  // MINDSPORE_CCSRC_VM_TRANSFORM_H_
