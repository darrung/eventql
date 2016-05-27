/**
 * Copyright (c) 2016 zScale Technology GmbH <legal@zscale.io>
 * Authors:
 *   - Paul Asmuth <paul@zscale.io>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License ("the license") as
 * published by the Free Software Foundation, either version 3 of the License,
 * or any later version.
 *
 * In accordance with Section 7(e) of the license, the licensing of the Program
 * under the license does not imply a trademark license. Therefore any rights,
 * title and interest in our trademarks remain entirely with us.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you develop
 * commercial activities involving this program without disclosing the source
 * code of your own applications
 */
#pragma once
#include "eventql/eventql.h"
#include "eventql/util/protobuf/msg.h"
#include "eventql/util/SHA1.h"

namespace eventql {

class MetadataFile : public RefCounted {
public:

  struct PartitionMapEntry {
    String begin;
    uint64_t placement_id;
    Vector<String> servers;
    Vector<String> servers_joining;
    bool splitting;
    String split_point;
    Vector<String> split_servers_low;
    Vector<String> split_servers_high;
  };

  MetadataFile(
      const SHA1Hash& transaction_id,
      const Vector<PartitionMapEntry>& partition_map);

  const SHA1Hash& getTransactionID() const;

  const Vector<PartitionMapEntry>& getPartitionMap() const;

protected:
  SHA1Hash transaction_id_;
  Vector<PartitionMapEntry> partition_map_;
};


} // namespace eventql