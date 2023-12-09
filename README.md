# NetBSD_ssh_chatbox
Anonymous ssh-based chatbox.
Tested on NetBSD 9.3
Compile using cc without further parameters.

This program should be considered experimental / proof of concept.

preparation:
* create user anonymous
* make this program the shell of user anonymous, using chsh -s [filename] anonymous
* create file /var/shm/box.txt and make it rw accessible for user anonymous using chmod ug+rwx [filename]

Still looking for something in NetBSD 9.3 to get nice timestamps with milliseconds. This program filters sysctl kern.cd_time for a timecounter but it sucks.
