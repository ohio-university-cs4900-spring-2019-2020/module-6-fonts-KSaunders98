# Module 6: Fonts
This module demonstrates the use of fonts in the engine, both screen-space and world-space. One font is placed above the other player's camera model and is adjusted to always point towards the camera on the XY plane. The other font is a chatbox at the bottom of the screen for sending messages to the other player.
## Instructions
- First, run one instance of the module with NetServerListenPort=12683 in the aftr.conf file and then run another instance with NetServerListenPort=12682 in the aftr.conf file.
- Move the camera around in either of the instances. Each instance sees the other one's camera model along with a nametag.
- Press enter, type a message, then press enter again to send a message to the other client. The message is displayed in the player's nametag.