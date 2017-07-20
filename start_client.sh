#!/bin/bash
./bin/client ./etc/config_client.lua
tail -n0 -F logger/log_client.txt
