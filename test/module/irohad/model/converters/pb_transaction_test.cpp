/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include "commands.pb.h"
#include "model/converters/pb_transaction_factory.hpp"
#include "model/transaction.hpp"

#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/add_signatory.hpp"
#include "model/commands/create_account.hpp"
#include "model/commands/create_asset.hpp"
#include "model/commands/create_domain.hpp"
#include "model/commands/remove_signatory.hpp"
#include "model/commands/set_permissions.hpp"
#include "model/commands/set_quorum.hpp"
#include "model/commands/transfer_asset.hpp"

TEST(TransactionTest, tx_test) {
  auto orig_tx = iroha::model::Transaction();
  orig_tx.creator_account_id = "andr@kek";
  auto siga = iroha::model::Signature();
  std::fill(siga.pubkey.begin(), siga.pubkey.end(), 0x22);
  std::fill(siga.signature.begin(), siga.signature.end(), 0x10);
  orig_tx.signatures = {siga};

  orig_tx.created_ts = 2;
  orig_tx.tx_counter = 1;

  auto c1 = iroha::model::CreateDomain();
  c1.domain_name = "keker";
  auto c2 = iroha::model::CreateAsset();
  c2.domain_id = "keker";
  c2.precision = 2;
  c2.asset_name = "fedor-coin";

  auto c3 = iroha::model::SetAccountPermissions();
  c3.account_id = "fedor";
  c3.new_permissions.can_transfer = true;
  c3.new_permissions.create_assets = true;

  orig_tx.commands = {
      std::make_shared<iroha::model::CreateDomain>(c1),
      std::make_shared<iroha::model::CreateAsset>(c2),
      std::make_shared<iroha::model::SetAccountPermissions>(c3)};

  auto factory = iroha::model::converters::PbTransactionFactory();
  auto proto_tx = factory.serialize(orig_tx);
  switch (proto_tx.payload().commands().Get(0).command_case()){

    case iroha::protocol::Command::kAddAssetQuantity:break;
    case iroha::protocol::Command::kAddPeer:break;
    case iroha::protocol::Command::kAddSignatory:break;
    case iroha::protocol::Command::kCreateAsset:break;
    case iroha::protocol::Command::kCreateAccount:break;
    case iroha::protocol::Command::kCreateDomain:break;
    case iroha::protocol::Command::kRemoveSign:break;
    case iroha::protocol::Command::kSetPermission:break;
    case iroha::protocol::Command::kSetQuorum:break;
    case iroha::protocol::Command::kTransferAsset:break;
    case iroha::protocol::Command::COMMAND_NOT_SET:break;
  }
  auto serial_tx = factory.deserialize(proto_tx);
  ASSERT_EQ(orig_tx, *serial_tx);
}
