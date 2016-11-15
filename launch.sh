#!/bin/bash
./socket_server.elf 45171 &
./socket_client.elf localhost 45171
