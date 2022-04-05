#ifndef TCP_COMMUNICATION_H
#define TCP_COMMUNICATION_H

void *read_from_distributed_server(void *args);
void *write_to_distributed_server(void *args);
void handle_received_messages(int msg_code);

#endif
