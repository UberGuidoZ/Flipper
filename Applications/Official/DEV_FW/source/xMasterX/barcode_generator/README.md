<p align="center">
	<h1 align="center">Barcode Generator</h1>  
  <p align="center">

A barcode generator for the Flipper Zero that supports **UPC-A**, **EAN-8**, **EAN-13**, **Code-39**, and **Code-128**[1]
</p>


## Table of Contents
- [Table of Contents](#table-of-contents)
- [Installing](#installing)
- [Usage](#usage)
  - [Creating a barcode](#creating-a-barcode)
  - [Editing a barcode](#editing-a-barcode)
  - [Deleting a barcode](#deleting-a-barcode)
  - [Viewing a barcode](#viewing-a-barcode)
- [Screenshots](#screenshots)
- [Credits](#credits)


## Installing
1) Download the `.zip` file from the release section
2) Extract/unzip the `.zip` file onto your computer
3) Open qFlipper and go to the file manager
4) Navigate to the `apps` folder
5) Drag & Drop the `.fap` file into the `apps` folder
6) Navigate back to the root folder and create the folder `app_data`, if not already there
7) Navigate into `app_data` and create another folder called `barcode_data`
8) Navigate into `barcode_data`
9) Drag & Drop the encoding txts (`code39_encodings.txt` & `code128_encodings.txt`) into the `barcode_data` folder


## Usage

### Creating a barcode
1) To create a barcode click on `Create Barcode`
2) Next select your type using the left and right arrows
3) Enter your filename and then your barcode data
4) Click save

### Editing a barcode
1) To edit a barcode click on `Edit Barcode`
2) Next select the barcode file you want to edit
3) Edit the type, name, or data
4) Click save

### Deleting a barcode
1) To delete a barcode click on `Edit Barcode`
2) Next select the barcode file you want to delete
3) Scroll all the way to the bottom
4) Click delete

### Viewing a barcode
1) To view a barcode click on `Load Barcode`
2) Next select the barcode file you want to view

## Screenshots
![Barcode Create Screen](screenshots/Creating%20Barcode.png "Barcode Create Screen")

![Flipper Code-128 Barcode](screenshots/Flipper%20Barcode.png "Flipper Code-128 Barcode")

![Flipper Box EAN-13 Barcode](screenshots/Flipper%20Box%20Barcode.png "Flipper Box EAN-13 Barcode")

## Credits

[Kingal1337](https://github.com/Kingal1337) - Developer

[@teeebor](https://github.com/teeebor) - Menu Code Snippet


[1] - Only supports Set B and only the characters from 0-94