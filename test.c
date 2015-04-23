
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <fcntl.h>


int main(int argc, char **argv){

     int ch,fd;
     char *ip;
     char *port;
     char *user;
     while ((ch = getopt(argc, argv, "i:p:u:")) != -1) {
             switch (ch) {
             case 'i':
                     if ((fd = open(optarg, O_RDONLY, 0)) < 0) {
                             printf("my ip is: %s\n", optarg);
                                
                             //ip = optarg;
                     }
                     break;
                     
            case 'p':
                    if ((fd = open(optarg, O_RDONLY, 0)) < 0) {
                             printf("my port number is: %s\n", optarg);
                             //port = optarg;
                          
                     }
                     break;
                     
            case 'u':
                    if ((fd = open(optarg, O_RDONLY, 0)) < 0) {
                             printf("my name is: %s\n", optarg);
                             //user = optarg;
                            
                     }
                     break;
             default:break;
             }
     }
     argc -= optind;
     argv += optind;
     return 0;
  }
