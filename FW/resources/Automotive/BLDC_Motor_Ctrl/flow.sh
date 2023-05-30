#!/bin/bash

exit 0

./waf configure

./waf build size upload --app=04

./waf debug_uart --port=0
