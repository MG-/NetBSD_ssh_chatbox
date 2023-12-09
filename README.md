# NetBSD_ssh_chatbox
ssh-based chatbox
Tested on NetBSD 9.3
Compile using cc without further parameters.

preparation:
* create user anonymous
* make this program the shell of user anonymous, using chsh -s [filename] anonymous
* create file /var/shm/box.txt and make it rw accessible for user anonymous using chmod ug+rwx [filename]
