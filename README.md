# PETAL
## Person Eye Tracking Application Launcher

__TOC__

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





