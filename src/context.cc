/* Copyright 2021 NVIDIA Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "context.h"
#include "legion.h"

namespace legate {

using namespace Legion;

LegateContext::LegateContext(Legion::Runtime *runtime,
                             const std::string &library_name,
                             const ResourceConfig &config)
{
  task_scope_ = ResourceScope(
    runtime->generate_library_task_ids(library_name.c_str(), config.max_tasks), config.max_tasks);
  mapper_scope_ =
    ResourceScope(runtime->generate_library_mapper_ids(library_name.c_str(), config.max_mappers),
                  config.max_mappers);
  redop_scope_ = ResourceScope(
    runtime->generate_library_task_ids(library_name.c_str(), config.max_reduction_ops),
    config.max_reduction_ops);
  proj_scope_ = ResourceScope(
    runtime->generate_library_reduction_ids(library_name.c_str(), config.max_projections),
    config.max_projections);
  shard_scope_ = ResourceScope(
    runtime->generate_library_sharding_ids(library_name.c_str(), config.max_shardings),
    config.max_shardings);
}

TaskID LegateContext::get_task_id(int64_t local_task_id) const
{
  assert(task_scope_.valid());
  return task_scope_.translate(local_task_id);
}

MapperID LegateContext::get_mapper_id(int64_t local_mapper_id) const
{
  assert(mapper_scope_.valid());
  return mapper_scope_.translate(local_mapper_id);
}

ReductionOpID LegateContext::get_reduction_op_id(int64_t local_redop_id) const
{
  assert(redop_scope_.valid());
  return redop_scope_.translate(local_redop_id);
}

ProjectionID LegateContext::get_projection_id(int64_t local_proj_id) const
{
  if (local_proj_id == 0)
    return 0;
  else {
    assert(proj_scope_.valid());
    return proj_scope_.translate(local_proj_id);
  }
}

ShardingID LegateContext::get_sharding_id(int64_t local_shard_id) const
{
  assert(shard_scope_.valid());
  return shard_scope_.translate(local_shard_id);
}

int64_t LegateContext::get_local_task_id(TaskID task_id) const
{
  assert(task_scope_.valid());
  return task_scope_.invert(task_id);
}

int64_t LegateContext::get_local_mapper_id(MapperID mapper_id) const
{
  assert(mapper_scope_.valid());
  return mapper_scope_.invert(mapper_id);
}

int64_t LegateContext::get_local_reduction_op_id(ReductionOpID redop_id) const
{
  assert(redop_scope_.valid());
  return redop_scope_.invert(redop_id);
}

int64_t LegateContext::get_local_projection_id(ProjectionID proj_id) const
{
  if (proj_id == 0)
    return 0;
  else {
    assert(proj_scope_.valid());
    return proj_scope_.invert(proj_id);
  }
}

int64_t LegateContext::get_local_sharding_id(ShardingID shard_id) const
{
  assert(shard_scope_.valid());
  return shard_scope_.invert(shard_id);
}

bool LegateContext::valid_task_id(Legion::TaskID task_id) const
{
  return task_scope_.in_scope(task_id);
}

bool LegateContext::valid_mapper_id(Legion::MapperID mapper_id) const
{
  return mapper_scope_.in_scope(mapper_id);
}

bool LegateContext::valid_reduction_op_id(Legion::ReductionOpID redop_id) const
{
  return redop_scope_.in_scope(redop_id);
}

bool LegateContext::valid_projection_id(Legion::ProjectionID proj_id) const
{
  return proj_scope_.in_scope(proj_id);
}

bool LegateContext::valid_sharding_id(Legion::ShardingID shard_id) const
{
  return shard_scope_.in_scope(shard_id);
}

}  // namespace legate
