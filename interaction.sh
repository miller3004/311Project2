#!/user/bin/env bash

# server variables
#
SERVER="text-server"
SERVER_MSG="server_out.txt"
SERVER_LOG="server_log.txt"
SERVER_FILES="$SERVER $SERVER_MSG $SERVER_LOG"

# client variables
#
CLIENT="text-client"
CLIENT_MSG="client_out.txt"
CLIENT_LOG="client_log.txt"
CLIENT_FILES="$CLIENT $CLIENT_MSG $CLIENT_LOG"

# execution parameters
SOCKET_NAME="proj2_socket_name"

FILE="dat/anna_karenina.txt"
SEARCH_STR="you m"

# clean things up before beginning
#
rm $SERVER_FILES
rm $CLIENT_FILES
make clean

# build and start server
#
make $SERVER
./$SERVER $SOCKET_NAME 1> $SERVER_MSG 2> $SERVER_LOG &
# remember PID to kill later
SERVER_PID=$!

# build and start client
#
make $CLIENT
./$CLIENT $SOCKET_NAME $FILE "$SEARCH_STR" 1> $CLIENT_MSG 2> $CLIENT_LOG

# kill server
#
kill $SERVER_PID
