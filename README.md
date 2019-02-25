# PETAL
## Person Eye Tracking Application Launcher

### Project Purpose
The goal of this project is to make a simple headset that can be combo'd with a windows application anyone can 
install on their windows computer.

### Issues

If you are about to work on something related to this any help is appreciated but it would be most helpful for
you to work off of one of the issues logged on github. You can find the repo here: 
[PETAL REPO](https://github.com/dschonholtz/PETAL)

To best help us track issues and to make sure people don't work on multiple issues at once if you pull a new
issue off of the backlog (TODO) move the associated ticket from the todo column to the in progress column and 
assign yourself to the ticket.

### Running the application

We now have a couple of different apps in our git repo and are still
in the process of integrating some of them.

For the main parent app that currently does the eye tracking and that the 
other apps will eventually be integrated into. Here are a couple of pointers
to help you get started.

1. Make sure that Boost and OpenCV are installed in C:/local/ as that is where the new
cmake file will reference them from. You also need the visual studio 2015 build tools 
which can be found in the visual studio installer with the tag v140.

		Boost should be named: boost_1_62_0
		and OpenCV should be named: opencv-3.1.0

2. Pull the repo down from git and then go into the directory:

        PETAL\src\3D-Eye-Tracker
     
3. In that directory find any and all paths that look like:

        C:/Users/schon/Documents/2019/School/Capstone/PETAL/src/3D-Eye-Tracker....
   
and modify them so they point to your local repo on your machine.

4. Now you should open the CMake GUI and browse to your source and build folders so that both paths should
end in the following: 

        /Your/local/path/here/PETAL/src/3D-Eye-Tracker
        /Your/local/path/here/PETAL/src/3D-Eye-Tracker/build
        
5. Then click File -> delete cache

6. Click the generate button

7. In the resulting pop up select the default option:

        Visual Studio 15 2017 Win64
        
8. BEFORE YOU EXIT THAT WINDOW enter the following option into the toolset window

        v140
        
9. Click finish and pray it doesn't error

10. Open the solution in the build folder 

        PETAL\src\3D-Eye-Tracker\build\3d_eye_tracker.sln
        
11. Make sure main is selected as the default project and not ALL_BUILD

12. Click the green debug button and it should all launch.
        
### Using git properly

Using git properly to make sure that no work is lost and everyone can work on the same project is essential 
when working as a team.

With this in mind, here are a few recomendations:

* Install [meld](http://meldmerge.org/) and use it as your merge and diff tools. To do this you can download and run
installer if you are on windows or follow the instructions on there site for other OS's.

Now you will need to modify your git config. I have found that the easiest way to do this is with Git Bash which you
can get from [here](https://gitforwindows.org/)

Using the git bash terminal you will be able to run linux like commands and interact with git in a terminal custom
built for that purpose.

If you are on windows you will need to add an environment variable for Home and set it to the path you want to be
represented as home in your git experience. This should be relatively high level. Like your documents folder 
C:/Users/{Username}/

With that home environment variable set, restart git bash then type:

    cd ~/
    vi .gitconfig
    
Then add the following to that gitconfig file:

    [merge]
        tool = meld
    [mergetool "meld"]
        path = C:/Program Files (x86)/Meld/Meld.exe
    [diff]
        tool = meld
    [difftool "meld"]
        path = C:/Program files (x86)/Meld/Meld.exe

When you start working on a new feature or ticket you should make a new branch off of master.
Then when you are ready to make a pull request to merge you changes into master you should do a rebase or a merge.
When you have to deal with merge conflicts you will now be able to work through that with meld instead of on the command
line.

Once the issue has been merged or rebased and you have retested your code to make sure it still works with the
changes created from the merge conflicts then you should be all set to put in a pull request.


## Working with CLI

Start here: 
https://www.codeproject.com/Articles/19354/Quick-C-CLI-Learn-C-CLI-in-less-than-10-minutes

Then look at the code in NativeCpp and in wrappers to see how the application goes Form->Wrapper->NativeCpp.





