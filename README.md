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

> Variable a has value:10
>
> and lives at address 0x3FFC00C8


We can see here that the value of the variable was printed along with it's address ``0x3FFC00C8`` in Hexadecimal. We can now dive into the [[ESP32 Technical Reference Manual (TRM)](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)] to find out what this address region corresponds to in the device we will be working with on this course, the ESP32 System-on-Chip (SoC).

On Page 27 of the TRM we can see the following chart.

![](imgs/address_map_general.png)

What we can see from the chart that our variable ``a`` which has address ``0x3FFC00C8`` lies in the region ``0x3FF80000 - 0x3FFFFFFF`` which maps into the ``512KB Embedded Memory`` of our SoC. Scrolling down a bit further we can find a further breakdown of the address space for the embedded memory.

![](imgs/embedded_memory.png)

From this we can see that our address ``0x3FFC00C8`` is in the region ``0x3FFAE0000 - 0x3FFDFFFF`` which is the ``Internal SRAM 2``, a very fast memory quite close to the CPU in our SoC.

### Derefrencing a pointer

We can also manipulate the data stored at the address a pointer point to. To use pointers in this way we need to do something called derefrencing.

Take the following example:

```C
int a; // a variable
int *ptr_a; // a variable that is a pointer

void setup() {
        Serial.begin(115200);
        Serial.print("\n\n");

        a = 10;
        ptr_a = &a; // ptr is pointing to the address of a

        *ptr_a = 40;

        Serial.print("Dereferencing a = ");
        Serial.println(*ptr_a);
}
```

This example is almost the same as the previous one, again we have ``a`` and ``ptr_a``. However, now we also have some other operations again using the ``*`` operator.

In the line ``*ptr_a = 40;`` we are using the ``*`` operator to assign the value 40 __into the location that the pointer is pointing to__.
In this case ``ptr_a`` is pointing at the memory location of variable ``a``, so the line of code ``*ptr_a = 40;`` overwrites the ``10`` already stored in ``a`` with ``40``. Essentially what is happening here, is the compiler is issuing a load to get the address stored in ``ptr_a``, it is then immediately using the address returned to perform a store operation with the literal value ``40``. 

A similar story can be said for dereferencing a pointer for reading purposes. In the line ``Serial.print(*ptr_a);`` the ``*ptr_a`` is telling the compiler, load the value stored in ``ptr_a`` then immediately use that returned value to perform another load, getting the contents of the ``a`` variable. 

This dual use of the ``*`` operator in the C syntax can be a source of confusion for people new to pointers, essentially:
* ``*`` is used to:
  * __declare__ a pointer type, e.g. ``int * ptr_a;`` 
  * __dereference__ a pointer, either for reading purposes ``Serial.print(*ptr_a)``, or for writing ``*ptr_a = 40``.
* ``&`` is used to get the address of a variable or object
