# Noiseless-Channel-Selective-ARQ
Write a C program to implement Selective Repeat ARQ in noisy channel. The sender should send more than one data frames(within window size) and start  timer for each and individual frame. If it receives acknowledgment, then it should stop the corresponding timer, move its sending window and send next set of  frame/frames. If sender receives negative acknowledgment or if the timer times out for a particular frame,  it should retransmit  that/those frames only. The receiver should send acknowledgment when it receives the frame that it expects. If receiver receives frame out of order, it should buffer them and send negative acknowledgment  for the lost frame.

This C Program uses the Concept of Socket and Can be run in a Linux OS. 
