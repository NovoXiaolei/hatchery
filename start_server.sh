#!/bin/bash

start_server(){
    printf "start server\n"
    ./bin/hatchery ./etc/config_server.lua
    while(true)
    do 
        if test $(ps ux | grep ${PID} | grep -v 'grep' | wc -l ) -eq 0
        then 
            printf "."
            sleep 1
        else 
            sleep 1
            break
        fi
    done
    printf "\n"
}

stop_server()
{
    printf "stop server\n"
    PID='cat server.pid'
    pid=`ps ux | grep ${PID} | grep -v 'grep' | awk '{if(NR==1)print $2}'`
    if [ "$pid" != "" ]; then 
        kill $pid 
        while(true)
        do 
            if test $(ps ux | grep ${pid} | grep -v 'grep' | wc -l ) -eq 0
            then 
                sleep 1
                break
            else 
                print "."
                sleep 1
            fi
        done
    fi 
    printf "\n"
}

tail_server(){
    tail -n0 -F ./logger/log_server.txt
}

case $1 in
    start)
        start_server 
        ;;
    stop)
        stop_server
        ;;
    tail)
        tail_server 
        ;;
    *)
        stop_server 
        start_server 
        ;;
esac
