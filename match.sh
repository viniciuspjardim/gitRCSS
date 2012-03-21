#!/bin/bash

set -e # to make sure nothing go wrong

# Check if it's the only script running
if pidof -x $(basename $0) > /dev/null; then
  for p in $(pidof -x $(basename $0)); do
    if [ $p -ne $$ ]; then
      echo "Script $0 is already running: exiting"
      exit 1
    fi
  done
fi

# Not needed but it's nice (Get the real path of the current directory)
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ] ; do SOURCE="$(readlink "$SOURCE")"; done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

SERVER="rcssserver"
T1EXEC=""
T2EXEC=""
T1WAIT=false
T2WAIT=false
LOGFOLDER="/home/vinicius/gitRCSS/logs" # <-- Folder to store the rcg and rcl logs

start_server()
{
    echo "Starting server..."
    cd $LOGFOLDER
    $SERVER server::synch_mode=off CSVSaver::save=on server::auto_mode=on server::game_over_wait=2 server::game_log_compression=1 server::text_log_compression=1 &
    cd $DIR
    return
}

start_monitor()
{
    echo "Starting monitor..."
    cd $LOGFOLDER
    rcssmonitor --auto-quit-mode on &
    cd $DIR
    return
}
# --

# -- Team 1
start_team1()
{
    T1EXEC="sample_player"                 # <-- Player executable name
    T1WAIT=true                            # <-- Do you want to wait all players stop?
    cd /home/vinicius/gitRCSS/iBots_0.00/src # <-- Script folder
    sh start.sh -t "iBots" &> /dev/null & # <-- Execute script
    
    cd $DIR
    return
}
stop_team1()
{
    killall $T1EXEC &> /dev/null || true
    #  _____ kill something else? # <--
    # \/
    killall "sample_coach" &> /dev/null || true   
    killall "sample_trainer" &> /dev/null || true
    # -
    return
}
# --

# -- Team 2
start_team2()
{
    T2EXEC="sample_player"               # <-- Player executable name
    T2WAIT=false                  # <-- Do you want to wait all players stop?
    cd /home/vinicius/gitRCSS/agent2d-3.1.0/src  # <-- Script folder
    sh start.sh -t "agent2d" &> /dev/null &    # <-- Execute script
    
    cd $DIR
    return
}
stop_team2()
{
    killall $T2EXEC &> /dev/null || true
    #  _____ kill something else? # <--
    # \/
    
    # -
    return
}
# --

start()
{
    start_server
    sleep 4
    start_monitor
    sleep 4
    echo "Starting teams..."
    start_team1
    sleep 1
    start_team2
    return
}

check()
{
    sec=888 # <------ MAX time for the match (sec)    
    
    echo "Checking..."
    wt=`awk "BEGIN{ print $sec / 2}"`
    for i in `seq 1 $wt`
    do
        sleep 2
        if ! killall -0 $SERVER ; then  return ; fi
        if [ `pidof $T1EXEC | wc -w` -le 7 ] ; then  return ; fi # <-- set how many team 1 player must be on (def:7)
        if [ `pidof $T2EXEC | wc -w` -le 7 ] ; then  return ; fi # <-- set how many team 2 player must be on (def:7)
    done
    echo "$sec seconds and the game is still on!?! Kill it with fire..."
    return 
}

finalize()
{
    echo "Stopping server..."
    killall $SERVER &> /dev/null || true
    sleep 3
    echo "Waiting players to save all the stuff..."
    sleep 4
    for i in {1..60} # <--- MAX time to wait the players to finnish (sec)
    do
        sleep 1
        if [ $T1WAIT ] && [ killall -0 $T1EXEC &> /dev/null ]
        then
            continue
        fi
        if  [ $T2WAIT ] && [ killall -0 $T2EXEC &> /dev/null ]
        then
            continue
        fi
        break
    done
    echo "Stopping players..."
    stop_team1
    stop_team2
    return
}

quick_stop()
{
  killall $SERVER &> /dev/null || true
  killall $T1EXEC &> /dev/null || true
  killall $T2EXEC &> /dev/null || true
}

trap quick_stop 0 1 2 3 8 15

start
sleep 10
check
echo "Finishing the game..."
finalize
sleep 1
echo "Match finished!"

