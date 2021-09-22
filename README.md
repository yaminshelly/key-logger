# key-logger
key loggering is a recording of each key that the user presses on a  keyboard and saving the data in a text file.

# Getting Started
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
