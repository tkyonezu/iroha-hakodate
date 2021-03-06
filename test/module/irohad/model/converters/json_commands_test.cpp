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

#include <algorithm>

#include <gtest/gtest.h>
#include <rapidjson/writer.h>

#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/add_signatory.hpp"
#include "model/commands/append_role.hpp"
#include "model/commands/create_account.hpp"
#include "model/commands/create_asset.hpp"
#include "model/commands/create_domain.hpp"
#include "model/commands/create_role.hpp"
#include "model/commands/grant_permission.hpp"
#include "model/commands/remove_signatory.hpp"
#include "model/commands/revoke_permission.hpp"
#include "model/commands/set_permissions.hpp"
#include "model/commands/set_quorum.hpp"
#include "model/commands/transfer_asset.hpp"
#include "model/converters/json_command_factory.hpp"

using namespace rapidjson;
using namespace iroha;
using namespace iroha::model;
using namespace iroha::model::converters;

class JsonCommandTest : public ::testing::Test {
 public:
  JsonCommandFactory factory;

  void command_converter_test(std::shared_ptr<Command> abstract_command) {
    auto json_repr = factory.serializeAbstractCommand(abstract_command);
    auto model_repr = factory.deserializeAbstractCommand(json_repr);
    ASSERT_TRUE(model_repr.has_value());
    ASSERT_EQ(*abstract_command, *model_repr.value());
  }
};

TEST_F(JsonCommandTest, ClassHandlerTest) {
  std::vector<std::shared_ptr<Command>> commands = {
      std::make_shared<AddAssetQuantity>(),
      std::make_shared<AddPeer>(),
      std::make_shared<AddSignatory>(),
      std::make_shared<CreateAccount>(),
      std::make_shared<CreateAsset>(),
      std::make_shared<CreateDomain>(),
      std::make_shared<RemoveSignatory>(),
      std::make_shared<SetAccountPermissions>(),
      std::make_shared<SetQuorum>(),
      std::make_shared<TransferAsset>()};
  for (const auto &command : commands) {
    auto ser = factory.serializeAbstractCommand(command);
    auto des = factory.deserializeAbstractCommand(ser);
    ASSERT_TRUE(des.has_value());
    ASSERT_EQ(*des.value(), *command);
  }
}

TEST_F(JsonCommandTest, InvalidWhenUnknownCommandType) {
  auto cmd = R"({
    "command_type": "Unknown",
    "account_id": "admin@test",
    "asset_id": "usd#test",
    "amount": {
        "int_part": -20,
        "frac_part": 0
    }
  })";

  auto json = stringToJson(cmd);
  ASSERT_TRUE(json.has_value());
  ASSERT_FALSE(factory.deserializeAbstractCommand(json.value()).has_value());
}

TEST_F(JsonCommandTest, add_asset_quantity) {
  auto orig_command = std::make_shared<AddAssetQuantity>();
  orig_command->account_id = "23";
  iroha::Amount amount(150, 2);

  orig_command->amount = amount;
  orig_command->asset_id = "23";

  auto json_command = factory.serializeAddAssetQuantity(orig_command);
  auto serial_command = factory.deserializeAddAssetQuantity(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());
  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, add_peer) {
  auto orig_addPeer = std::make_shared<AddPeer>();
  orig_addPeer->address = "10.90.129.23";
  auto proto_add_peer = factory.serializeAddPeer(orig_addPeer);
  auto serial_addPeer = factory.deserializeAddPeer(proto_add_peer);

  ASSERT_TRUE(serial_addPeer.has_value());
  ASSERT_EQ(*orig_addPeer, *serial_addPeer.value());
  command_converter_test(orig_addPeer);

  orig_addPeer->address = "134";
  ASSERT_NE(*serial_addPeer.value(), *orig_addPeer);
}

TEST_F(JsonCommandTest, add_signatory) {
  auto orig_command = std::make_shared<AddSignatory>();
  orig_command->account_id = "23";

  auto json_command = factory.serializeAddSignatory(orig_command);
  auto serial_command = factory.deserializeAddSignatory(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());
  command_converter_test(orig_command);

  orig_command->account_id = "100500";
  ASSERT_NE(*orig_command, *serial_command.value());
}

TEST_F(JsonCommandTest, add_signatory_abstract_factory) {
  auto orig_command = std::make_shared<AddSignatory>();
  orig_command->account_id = "23";

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, create_asset) {
  auto orig_command = std::make_shared<CreateAsset>();
  orig_command->domain_id = "kek_cheburek";
  orig_command->precision = 1;
  orig_command->asset_name = "test_asset";

  auto json_command = factory.serializeCreateAsset(orig_command);
  auto serial_command = factory.deserializeCreateAsset(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, create_account) {
  auto orig_command = std::make_shared<CreateAccount>();
  orig_command->account_name = "keker";
  orig_command->domain_id = "cheburek";

  auto json_command = factory.serializeCreateAccount(orig_command);
  auto serial_command = factory.deserializeCreateAccount(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());
  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, remove_signatory) {
  auto orig_command = std::make_shared<RemoveSignatory>();
  orig_command->account_id = "Vasya";
  std::fill(orig_command->pubkey.begin(), orig_command->pubkey.end(), 0xF);

  auto json_command = factory.serializeRemoveSignatory(orig_command);
  auto serial_command = factory.deserializeRemoveSignatory(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, set_acount_permissions) {
  auto orig_command = std::make_shared<SetAccountPermissions>();
  orig_command->account_id = "Vasya";
  iroha::model::Account::Permissions perm;
  perm.can_transfer = true;
  perm.add_signatory = true;
  perm.issue_assets = true;
  orig_command->new_permissions = perm;

  auto json_command = factory.serializeSetAccountPermissions(orig_command);
  auto serial_command = factory.deserializeSetAccountPermissions(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, set_account_quorum) {
  auto orig_command = std::make_shared<SetQuorum>();
  orig_command->new_quorum = 11;
  orig_command->account_id = "Vasya";

  auto json_command = factory.serializeSetQuorum(orig_command);
  auto serial_command = factory.deserializeSetQuorum(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, set_transfer_asset) {
  auto orig_command = std::make_shared<TransferAsset>();
  orig_command->amount = {1, 20};
  orig_command->asset_id = "tugrik";
  orig_command->src_account_id = "Vasya";
  orig_command->dest_account_id = "Petya";
  orig_command->description = "from Vasya to Petya with love";

  auto json_command = factory.serializeTransferAsset(orig_command);
  auto serial_command = factory.deserializeTransferAsset(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, append_role) {
  auto orig_command = std::make_shared<AppendRole>("test@test", "master");
  auto json_command = factory.serializeAppendRole(orig_command);
  auto serial_command = factory.deserializeAppendRole(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, create_role) {
  std::vector<std::string> perms = {"CanDoMagic"};
  auto orig_command = std::make_shared<CreateRole>("master", perms);
  auto json_command = factory.serializeCreateRole(orig_command);
  auto serial_command = factory.deserializeCreateRole(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, grant_permission) {
  auto orig_command = std::make_shared<GrantPermission>("admin@test","can_read");
  auto json_command = factory.serializeGrantPermission(orig_command);
  auto serial_command = factory.deserializeGrantPermission(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}

TEST_F(JsonCommandTest, revoke_permission) {
  auto orig_command = std::make_shared<RevokePermission>("admin@test","can_read");
  auto json_command = factory.serializeRevokePermission(orig_command);
  auto serial_command = factory.deserializeRevokePermission(json_command);

  ASSERT_TRUE(serial_command.has_value());
  ASSERT_EQ(*orig_command, *serial_command.value());

  command_converter_test(orig_command);
}
