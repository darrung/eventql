/**
 * Copyright (c) 2016 DeepCortex GmbH <legal@eventql.io>
 * Authors:
 *   - Paul Asmuth <paul@eventql.io>
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
#include "eventql/util/status.h"
#include "eventql/util/thread/DelayedQueue.h"
#include "eventql/db/metadata_operation.h"
#include "eventql/db/metadata_file.h"
#include "eventql/db/metadata_store.h"
#include "eventql/config/config_directory.h"
#include "eventql/transport/native/client_tcp.h"

namespace eventql {

class MetadataReplication {
public:

  static const uint64_t kRetryDelayMicros = 30 * kMicrosPerSecond;

  MetadataReplication(
      ConfigDirectory* cdir,
      ProcessConfig* config,
      const String& server_name,
      MetadataStore* metadata_store,
      native_transport::TCPConnectionPool* conn_pool,
      net::DNSCache* dns_cache);

  ~MetadataReplication();

  void start();
  void stop();

protected:

  struct ReplicationJob {
    String db_namespace;
    String table_id;
    SHA1Hash transaction_id;
    Vector<String> servers;
  };

  void applyTableConfigChange(const TableDefinition& cfg);

  void replicateWithRetries(const ReplicationJob& job);
  Status replicate(const ReplicationJob& job);

  ConfigDirectory* cdir_;
  ProcessConfig* config_;
  String server_name_;
  MetadataStore* metadata_store_;
  native_transport::TCPConnectionPool* conn_pool_;
  net::DNSCache* dns_cache_;
  std::atomic<bool> running_;
  Vector<std::thread> threads_;
  thread::DelayedQueue<ReplicationJob> queue_;
};

} // namespace eventql
