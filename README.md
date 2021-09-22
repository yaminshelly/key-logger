# key-logger
key loggering is a recording of each key that the user presses on a  keyboard and saving the data in a text file.

## About
Our project had a requirement to save the file in kobject, ie in the / sys / 
folder.
The actions are usually performed behind the scenes and the user is not 
aware of this, which can lead to hacking of websites, saving user 
passwords and the like. Each button that the user presses is saved in the 
system, which means that when the keyboard is pressed for the 
characters of his password, the password will be saved in the file.
We will use Loadable Kernel Modules which are the ko files that will be 
created, and also Device Drivers stuff. Finally we will run the user 
application to show the data saved within the file.

## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites
- linux (ubunto)
- Terminal 
### commands
- Go to the folder where our file is located
```
cd shelly/adv_os/klogger_project
```

- We will run the make command
```
make
```
- We will check that a ko file has indeed been created
```
ls
```
- Insert the module into the kernel by the insmod command –
```
sudo insmod my_kobject.ko
```
- Insert password 
```
osboxes.org
```
- Compile the user application file
```
gcc user.c -o user_app
```
- Run the user_app :
```
./user_app
```
###### Now you in the user app , you can choose if you want to see the file
###### or manual like this with all the command : 
  - If you choose to see the file press 1 and enter .
  - If you want to see the manual press 2 and enter .
## Sources of information
bootlin - [bootlin](https://elixir.bootlin.com/linux/v5.14/source/drivers/tty/vt/keybo)

## Authors
Shelly Revivo - [shelly revivo](https://github.com/yaminshelly)

Tomer Revivo - [tomer revivo](https://github.com/TomerRevivo)





