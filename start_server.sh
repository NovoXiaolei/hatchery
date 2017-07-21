#!/bin/bash
./bin/hatchery ./etc/config_server.lua
tail -n0 -F ./logger/log_server.txt
