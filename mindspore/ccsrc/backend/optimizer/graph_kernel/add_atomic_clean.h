/**
 * Copyright 2020-2021 Huawei Technologies Co., Ltd
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

#ifndef MINDSPORE_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADD_ATOMIC_CLEAN_H_
#define MINDSPORE_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADD_ATOMIC_CLEAN_H_

#include <memory>
#include <tuple>
#include <utility>
#include <vector>
#include <string>
#include "backend/optimizer/common/optimizer.h"
#include "backend/session/kernel_graph.h"

namespace mindspore {
namespace opt {
struct AtomicAddInfo {
  CNodePtr atomic_add_node{nullptr};
  size_t reduce_real_output_index{0};
  size_t real_output_num{0};
};

class AtomicAddChecker {
 public:
  AtomicAddChecker() = default;
  virtual ~AtomicAddChecker() = default;
  static std::shared_ptr<AtomicAddChecker> Init();

  bool Check(const AnfNodePtr &node);
  AtomicAddInfo GetAtomicAddInfo() { return atomic_add_info_; }

 protected:
  virtual bool SuitableForAtomicAdd(const AnfNodePtr &node) { return false; }
  virtual bool FindCandidate(const AnfNodePtr &anf_node);
  virtual bool CanActivateAtomicAdd(const AnfNodePtr &anf_node);

 private:
  AtomicAddInfo atomic_add_info_;
};

class AtomicAddCheckerGPU : public AtomicAddChecker {
 public:
  AtomicAddCheckerGPU() = default;
  ~AtomicAddCheckerGPU() = default;

 protected:
  bool SuitableForAtomicAdd(const AnfNodePtr &node) override;
};

class AtomicAddCheckerAscend : public AtomicAddChecker {
 public:
  AtomicAddCheckerAscend() = default;
  ~AtomicAddCheckerAscend() = default;

 protected:
  bool SuitableForAtomicAdd(const AnfNodePtr &node) override;
};

class AtomicCleanInsertter : public Pass {
 public:
  explicit AtomicCleanInsertter(const std::string &name = "atomic_clean") : Pass(name) {}
  ~AtomicCleanInsertter() override = default;
  virtual bool Run(const FuncGraphPtr &func_graph);

 protected:
  virtual void CorrectKernelBuildInfo(const AnfNodePtr &composite_node, const AnfNodePtr &new_input);
  virtual void ProcessOriginCNode(const AnfNodePtr &composite_node, const AnfNodePtr &new_input,
                                  const FuncGraphManagerPtr &mng);
  void AddDepend(const FuncGraphPtr &main_graph, const AnfNodePtr &clean_node, const AnfNodePtr &composite_node,
                 const AnfNodePtr &user_node, int index);
  void InsertAtomicClean(const KernelGraphPtr &main_graph, const AnfNodePtr &anf_node, const FuncGraphManagerPtr &mng);
  CNodePtr InsertUpdateState(const KernelGraphPtr &main_graph, const CNodePtr &composite_node);
  CNodePtr atomic_add_node_{nullptr};

 private:
  void CorrectAbstract(const AnfNodePtr &composite_node);
  CNodePtr CreateAtomicCleanCompositeNode(const KernelGraphPtr &main_graph, TypeId dst_type);
  void CreateInplaceAssignNodeAndCorrectReturn(const FuncGraphPtr &sub_graph, const AnfNodePtr &new_parameter);
  void ProcessOriginCNodeUser(const KernelGraphPtr &main_graph, const AnfNodePtr &composite_node,
                              const AnfNodePtr &broadcast_to_node, const AnfNodePtr &update_state_node,
                              const FuncGraphManagerPtr &mng);
  std::vector<std::pair<AnfNodePtr, int>> FindOriginCNodeUsers(const KernelGraphPtr &main_graph,
                                                               const AnfNodePtr &composite_node,
                                                               const FuncGraphManagerPtr &mng, bool correct_index);
  bool IsExistStructuralObstacle(const KernelGraphPtr &main_graph, const AnfNodePtr &node,
                                 const FuncGraphManagerPtr &mng);

  size_t reduce_real_output_index_{0};
  size_t real_output_num_{0};
  std::vector<std::pair<AnfNodePtr, AnfNodePtr>> to_process_order_;
};
using AtomicCleanInsertterPtr = std::shared_ptr<AtomicCleanInsertter>;
}  // namespace opt
}  // namespace mindspore

#endif  // MINDSPORE_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADD_ATOMIC_CLEAN_H_
