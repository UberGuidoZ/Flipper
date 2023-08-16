# flipperzero-pong
A Pong game for the Flipper Zero

![](https://github.com/nmrr/flipperzero-pong/blob/main/img/Flipper_Zero.jpg)

Assuming the toolchain is already installed, copy **flipper_pong** directory to **applications_user**

Plug your **Flipper Zero** and build the Pong :
```
./fbt DEBUG=no LIB_DEBUG=no COMPACT=yes launch_app APPSRC=applications_user/flipper_pong
```

The game will automatically be launched after compilation

Because of the position of control pad on the **Flipper Zero**, this Pong has been programmed to be played in vertical position

Press **Up** or **Down** to move your paddle. Press back button to quit

If you don't want to build the game, just simply copy **flipper_pong.fap** on your **Flipper Zero** 

## Gallery ##

<img src="https://github.com/nmrr/flipperzero-pong/blob/main/img/flipper1.png" width=20% height=20%> <img src="https://github.com/nmrr/flipperzero-pong/blob/main/img/flipper2.png" width=10% height=10%> <img src="https://github.com/nmrr/flipperzero-pong/blob/main/img/flipper3.png" width=10% height=10%>
