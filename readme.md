# Multipurpose Server
## Multiprocess
You can make process(daemon) up according to function. For example, Logger, Dispatcher, Server and Worker.
The number of all the processes may be one or more. In the example, Logger, Dispatcher and Worker is one, Server is three. Dispatcher send message to Servers alternately.

## Multithread
Each process has several threads and each thread can have respective function. In the example, All the processes has common function.

## Plugin and FSM(Finite State Machine)
MM-server execute all functions via FSM by per execution unit. To add new function, You only need to make plugin(.so) and FSM config(.json).

## LOG(consol, file, email)
Logging system can write log to consol stream, file stream and send email. To configure logging, You only need to modify log config(.json).

## JSON Configuration
In MM-server, JSON can configure all the functions of process, thread, FSM and log.

## IPC
POSIX message queue, shared memory
socket, domain socket
named pipe

## Processes of example
MM-server's example is REST server and the function of each process is as follows.
1. Logger: logging important message.
2. Dispatcher: accept socket request and send fd to Server.
3. Server: receive fd from Dispatcher and functions as follows.
    1. process request and response via fd
    2. send socket request to Worker
4. Worker: receive request from Server, process request and response via fd

# Install and run
1. Install visual studio code.
2. Install extension - Remote Development, CMake
3. Connect Ubuntu 22.04 Server
4. Copy source to server's directory /your/path
5. Run "CMake: Build" in Command Pallete
6. Change directory to /your/path/run
7. execute "mmRun"

# Available example of REST client request
GET http://xxx.xxx.xxx.xxx:10004 HTTP/1.1

POST http://xxx.xxx.xxx.xxx:10004/basic/basic HTTP/1.1

POST http://xxx.xxx.xxx.xxx:10004/send HTTP/1.1
content-Type: application/json

{
    "value": 19170
}

GET http://xxx.xxx.xxx.xxx:10004/id/items/1234/5678/size HTTP/1.1

POST http://xxx.xxx.xxx.xxx:10004/python HTTP/1.1


# The merits of MM-server
## Simple and effective server for IOT
I coded MM-server under environment of Ubuntu 22.04 Server(LTS). It is used in Raspberry pi with Ubuntu 22.04 Server. So you can use as IOT's server. When I test in Raspberry pi 4, it works well as expected except for logging to email. To send logging email, logging system use google's SMTP. The process takes some time.

## Easy and efficient setting
Via JSON, you can configure all the functions.
```
logger.json
{
    "name_sem": "/mm?type_?##",
    "label": "?type_?##",
    "loggers": {
        "root": {
            "tag": "",
            "level": "trace",
            "size": 4096,
            "active": true,
            "propagate": false,
            "handlers": [
            ],
            "loggers": {
                "app": {
                    "tag": "app",
                    "level": "trace",
                    "size": 4096,
                    "active": true,
                    "propagate": false,
                    "handlers": [
                        "consol",
                        "file",
                        "email"
                    ]
                },
                "so": {
                    "tag": "so",
                    "level": "trace",
                    "size": 4096,
                    "active": true,
                    "propagate": false,
                    "handlers": [
                        "consol",
                        "file",
                        "email"
                    ]
                }
            }
        }
    },
    "handlers": {
        "consol": {
            "type": "consol",
            "level": "trace",
            "active": true,
            "stream": "stderr"
        },
        "file": {
            "type": "file",
            "level": "trace",
            "active": true,
            "path": "log/?yyyymmdd/?type_?##_?hhmmss_?uuuuuu.log",
            "critical": "error",
            "time_rotation": [[0, 0]],
            "capacity_rotation": 10240000
        },
        "email": {
            "type": "email",
            "level": "fatal",
            "active": true,
            "receivers": [
                "aha.mike.0018@gmail.com"
            ]
        }
    }
}
```
```
FSM.json
{
    "wait_message": {
        "req_plus_one": {
            "action": [
                "plugin/so/libtestServerWorker.so",
                "worker_receive_req_plus_one"
            ],
            "option": {
            },
            "next": {
                "ACK": "state_plus_one_second",
                "NAK": "state_final"
            }
        }
    },
    "state_plus_one_second": {
        "": {
            "action": [
                "plugin/so/libtestServerWorker.so",
                "worker_receive_plus_one_second"
            ],
            "option": {
            },
            "next": {
                "ACK": "state_final",
                "NAK": "state_final"
            }
        }
    },
    "state_final": {
        "": {
            "action": [
                "plugin/so/libtestServer.so",
                "state_final_default"
            ],
            "option": {
            },
            "next": {
                "ACK": null,
                "NAK": null
            }
        }
    }
}
```
```
immanence.json - configure REST
{
    "REST": {
        "basic": {
            "basic": {
                "\/POST": "basic"
            }
        },
        "send": {
            "\/POST": "req_plus_one"
        },
        "id": {
            "items": {
                "\\": {
                    "\\": {
                        "size": {
                            "\/GET": "get_volume"
                        }
                    }
                },
                "\/POST": "help"
            }
        },
        "python": {
            "\/POST": "execute_python"
        },
        "\/GET": "help"
    }
}
```

## Independent developing
Developers can code independently for each function. Although a developer take a mistake, all the other existing functions work well. Even if two developers code different functions at the same time, there is no problem. They only need to code to make plugin up independently and configure FSM(.json).
