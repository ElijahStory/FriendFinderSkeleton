# Friend Finder Skeleton
### NMD 342

This is the skeleton code for the FriendFinder project. The main file you will be writing in is [FriendFinderSkeleton.ino](https://github.com/jacobmealey/FriendFinderSkeleton/blob/main/FriendFinderSkeleton.ino)
You will need to write three functions, `match()`, `pack()` and `unpack()`. Along with the three functions mentioned before you will also need a struct called `friendfinder_data` which is a structure that will orgonize the sections of the friend finder system. 

### unpack() / unpack()
These two will be discussed and outlined as a class, depending on how we pack and unpack data is going to be very important in making sure all the Friend Finder devices communicate properly. 

`pack()` will be passed a `friendfinder_data` struct and return a `uint64_t` that is the packed data, where `unpack()` will take a `uint64_t` that is the packed data and return a `friendfinder_data` struct. 

### match()
Match is the function that is used for compairing one friend finder device with another. Match by default returns a bool (true or false) but if you wanted, you could also do it as levels of matching using an integer. 
