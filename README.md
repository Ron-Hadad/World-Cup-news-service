# World-Cup-news-service
## Overview
This project implements a World Cup news subscription service using the Stomp protocol. The server is implemented in Java with support for both TCP and Reactor design patterns. The client is implemented in C++. Clients can subscribe to channels, send reports, and receive reports from other clients on the same channel.

## How to use - The protocol
1. Login by: 'login {host:port} {username} {password}'
2. Example: login 1.1.1.1:2000 ron 1234

3. Subscribe to a Channel: 'join {game_name}'
Example: join Germany_Spain

4. Unsubscribe from a Channel: 'exit {game_name}'
Example: exit Germany_Spain

5. Send a Report to a Channel: 'report {report_file_name}'
Example: report Germany_Spain {events1_partial.json}
This will sent the 'events1_partial.json' file placed in the 'data' folder. Feel free to change the report as you like.

7. Logout: 'logout'

## Running the Server
To run the server, navigate to the server folder and execute the following command:
'mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="<7777> tcp"'
If the server starts successfully, you will get the message: 'Server started'.

## Running the Client
To run the client, navigate to the client folder and execute the 'make' command.
This will create an excecutable file named 'StompWCIClient' in the 'bin' folder. 
To start the client, excute the command:
'./bin/StompWCIClient 127.0.0.1 7777'
After executing these commands, you can start logging in, subscribing to channels, sending reports, and performing other actions.

You can see the massages recieved from tha server in the client prompt, as well as the messages recieved from the clients in the server prompt. 

## Dependencies
1. Linux environment
2. C++ environment
3. Maven package

Feel free to explore the code in the server and client folders for more details on the implementation.
If you have any feedback or quistions, please reach out!
Enjoy using the Stomp World Cup News Subscription Service!
