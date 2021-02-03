# Lecture 1: Lab Introduction & Memory Mapped Hardware 
In this lecture, we will look at Lab 1, and we will also investigate the hardware/software boundary.

### Arduino code
The technical content of this lecture is mainly around live coding demonstrations in the Arduino IDE (what you'll be using in labs). We will be using it to investigate pointers and manipulate the hardware registers of the device. The code examples that I follow can be found [[here](https://github.com/STFleming/EmSys_LabIntro_MemoryMappedHardware/tree/main/Arduino)]. _Note that they may differ slightly from what I code in the lecture_

# Lab Admin
### Lab Groups 
For the labs, you will need to arrange yourselves into groups of two or three. Once you have decided on the members of your group __one lab member__ should input the student IDs of each group member into the form found here [[form link](https://forms.office.com/Pages/ResponsePage.aspx?id=LrXKu76f1kOi859mxD3yaGURemXIkvJEi0OJyB0IdsxUNDNPUVFHWFdZSVFQMUhTTTNTRDRBSjBRNi4u)] by __Wednesday 03/02/2021__ before __15:00__ . After this time, any people left over will be randomly allocated into groups.

### Lab machine access
The EmSys labs will be using the Linux machines located in the Foundry. You should have received an email containing your login details to access these machines. __If you have not received your login details, please contact me__.

### Connecting to the Lab and Enviroment Setup 
Follow the guide [[here](https://github.com/STFleming/EmSys_labSetup)] on how to connect to the lab via ssh and setup your environment. 

### Lab1 repository
The Lab1 repository is available [[here](https://github.com/STFleming/EmSys_Lab1)]. Here you will find information on the lab tasks, the required structure of your logbook repository, and videos explaining how to use the tools.

### During the labs
The EmSys labs will use Discord to manage communications between groups and demonstrators. On Discord, there will be a voice channel will for each group. Each group member connects to their appropriate channel where they will be able to share their screens and complete the lab together. If you have a question, request assistance in the ``raise-your-hand`` channel, stating what room you are in; a demonstrator will come and join your voice channel and help you once they are free. Remember:  keep an eye on the ``lab-announcements`` channel for lab wide announcements.

# Memory mapped hardware

In the lecture, I discussed the concept of mapping hardware into the address space of the system. By providing memory addresses for hardware we create an incredibly powerful and convenient abstraction for manipulating hardware from software. Every microprocessor will have memory; memory needs addresses, so using the same convention our custom hardware component can be compatible with all systems. 

Now, dealing with memory is dangerous. For instance, it's easy to write into memory regions you shouldn't accidentally, or you can forget to free it when you've finished using it. For these reasons, and many others, most languages perform memory management for the programmer. However, C does not. In C, we have the idea of a pointer -- a variable that instead of storing a value, stores a memory location. The pointers provided by C allow us to manipulate memory directly, and thus we can also interact directly with memory-mapped hardware. It's this reason that almost all drivers and deep parts of operating systems use the C language, and one of the reasons it is still a prevalent language since its development in 1972. 

In this document, we will look at pointers and interfacing with our TinyPico devices. Specifically, we will investigate:
* C Pointers -- looking at examples in the Arduino IDE
* The address map of our TinyPico
* How we can read and write to hardware registers and interact with memory-mapped hardware
* Bit manipulation operations in C

### Pointers
Look at the following arduino sketch

```C

int a; // a variable
int *ptr_a; // a variable that is a pointer

void setup() {
        Serial.begin(115200);
        Serial.print("\n\n");

        a = 10;
        ptr_a = &a; // ptr is pointing to the address of a

        Serial.print("Variable a has value:");
        Serial.println(a);

        Serial.print("and lives at address 0x");
        Serial.println((unsigned int)ptr_a, HEX);
}
```
[[This code can be found here](Arduino/BasicPointer)]

In this sketch we have a few variables, ``a`` which is assigned the value ``10`` and ``ptr_a`` which is a pointer variable. To declare a pointer we need to use the ``*`` operator, we place this after the type in the variable declaration to make our variable a pointer of that type. In this example, we have ``int * ptr_a;`` which says that ``ptr_a`` is a pointer to an integer type. We can make pointers to any type we wish, for instance, ``float * ptr_b;`` would make a pointer to a floating point variable.  

In order to effectively use a pointer, we need to have the ability to lookup where items are stored in memory. To do this we can use the ``&`` operator. In the example above, we have the line ``ptr_a = &a``, what this effectively says is,_"get the address of where variable ``a`` is stored, and store that address in ``ptr_a``"_.

Let's compile the code and see what the output looks like on the ``Serial`` monitor in Arduino.

