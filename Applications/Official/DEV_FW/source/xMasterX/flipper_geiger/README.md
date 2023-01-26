# flipperzero-geigercounter
A geiger counter application for the Flipper Zero

![banner](https://github.com/nmrr/flipperzero-geigercounter/blob/main/img/logo.jpg)

You need a **geiger counter** board to run this application. This board can be used : https://aliexpress.com/item/1005004074447209.html

The geiger counter board can be powered with +5V power pin of the **Flipper Zero** 

Output pin for measure on arduino cannot be used on the **Flipper Zero** because output voltage is too low. You can use jack out port instead. This port must be connected on **A7** GPIO :

<p align="center"><img src="https://github.com/nmrr/flipperzero-geigercounter/blob/main/img/schematic.jpg" width=75% height=75%></p>

## Build the program

Assuming the toolchain is already installed, copy **flipper_geiger** directory to **applications_user**

Plug your **Flipper Zero** and build the geiger counter :
```
./fbt launch_app APPSRC=applications_user/flipper_geiger
```

The program will automatically be launched after compilation

<img src="https://github.com/nmrr/flipperzero-geigercounter/blob/main/img/flipper1.png" width=25% height=25%>

**A4** GPIO can be connected on **A7** GPIO to test this application without using a geiger tube. **A4** GPIO is generating a signal whose frequency changes every second.

Press Ok button to clear the graph, press back button to quit

If you don't want to build this application, just simply copy **flipper_geiger.fap** on your **Flipper Zero**

## Use cases

Ambient radioactivity :

<img src="https://github.com/nmrr/flipperzero-geigercounter/blob/main/img/flipper2.png" width=25% height=25%>

Measure of uranium ore piece inside a lead container :

<img src="https://github.com/nmrr/flipperzero-geigercounter/blob/main/img/flipper3.png" width=25% height=25%>

Measure of uranium ore piece in contact with the geiger tube :

<img src="https://github.com/nmrr/flipperzero-geigercounter/blob/main/img/flipper4.png" width=25% height=25%>

All previous measures in a row (the scale of the graph is automatically adjusted) :

<img src="https://github.com/nmrr/flipperzero-geigercounter/blob/main/img/flipper5.png" width=25% height=25%>

**A4** GPIO on **A7** GPIO :

<img src="https://github.com/nmrr/flipperzero-geigercounter/blob/main/img/flipper6.png" width=25% height=25%>

## Changelog

* 2023-01-15
  * Code fix & a lot of optimizations. Now a lot of events can be handled without any issue

* 2023-01-09
  * Schematic was added & code fix

* 2023-01-08
  * Initial release