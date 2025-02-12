#!/bin/bash
# set -x

sudo docker ps -a -f status=exited -q | xargs sudo docker start