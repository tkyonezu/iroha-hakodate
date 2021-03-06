# Copyright 2017 Soramitsu Co., Ltd. All Rights Reserved.
#
#---------------------------------------------------------------
# This makefile defines the following targets
#
# - all (default) - buid iroha-build container, and build iroha
# - docker        - build iroha runtime container
# - up            - running iroha container by docker-compose
# - down          - stop and remove iroha container by docker-compose
# - test          - exec all test commands
# - logs          - show logs of iroha_node_1 container
# - clean         - cleaning protobuf schemas and build directory
#
# - iroha-develop - build iroha-docker-develop  container
# - iroha-release - build iroha by iroha-docker-develop
# - iroha         - build iroha runtime container
#
# - iroha-up      - running iroha container by docker-compose
# - iroha-down    - stop and remove iroha container by docker-compose
#---------------------------------------------------------------
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

.PHONY: all help docker up dwon test clean

IROHA_HOME := /usr/local/iroha
BUILD_HOME := $(shell pwd)

UKERNEL := $(shell uname -s)
UMACHINE := $(shell uname -m)

ifeq ($(UKERNEL),Linux)
  ifeq ($(UMACHINE),x86_64)
    PROJECT := hyperledger
    DOCKER := Dockerfile
    COMPOSE := docker-compose.yml
  endif
  ifeq ($(UMACHINE),armv7l)
    PROJECT := arm32v7
    DOCKER := Dockerfile.arm32v7
    COMPOSE := docker-compose-arm32v7.yml
  endif
endif
ifeq ($(UKERNEL),Darwin)
  PROJECT := hyperledger
  DOCKER := Dockerfile
  COMPOSE := docker-compose.yml
endif

ifeq ($(DOCKER), )
$(error This platform "$(UKERNEL)/$(UMACHINE)" in not supported.)
endif

all: iroha-develop iroha-build iroha-release iroha

help:
	@echo "help          - show make targets"
	@echo "all (default) - buid iroha-build container, and build iroha"
	@echo "docker        - build iroha runtime container"
	@echo "up            - running iroha container by docker-compose"
	@echo "down          - stop and remove iroha container by docker-compose"
	@echo "test          - exec all test commands"
	@echo "logs          - show logs of iroha_node_1 container"
	@echo "clean         - cleaning protobuf schemas and build directory"

docker: iroha-release iroha

up: iroha-up

down: iroha-down

iroha-develop:
	cd docker/develop && docker build --rm -t $(PROJECT)/iroha-docker-develop -f $(DOCKER) .

iroha-build:
	docker run -t --rm --name iroha -v $(BUILD_HOME):/usr/local/iroha -w /usr/local/iroha $(PROJECT)/iroha-docker-develop /usr/local/iroha/scripts/iroha-build.sh

iroha-release:
	docker run -t --rm --name iroha-release -v $(BUILD_HOME):/usr/local/iroha -w /usr/local/iroha $(PROJECT)/iroha-docker-develop /usr/local/iroha/scripts/iroha-release.sh

iroha:
	cd docker/release && docker build --rm -t $(PROJECT)/iroha -f $(DOCKER) .

iroha-up:
	cd docker && env COMPOSE_PROJECT_NAME=iroha docker-compose -p iroha -f $(COMPOSE) up -d

iroha-down:
	cd docker && env COMPOSE_PROJECT_NAME=iroha docker-compose -p iroha -f $(COMPOSE) down

test:
	cd scripts && bash iroha-test.sh

logs:
	docker logs -f iroha_node_1

clean:
	-rm schema/*.{cc,h}
	rm -fr build
