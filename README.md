This is a GUI for iptables.

It will generate a bash script based on the user's modifications in the GUI. It can be exported to a remote server or executed locally.


todo - would like to see this thing ask for user permissions by using something like this 

#!/bin/sh
#put in systemd script for auto boot

iptables -F
iptables -X
iptables -P INPUT DROP
iptables -P OUTPUT DROP
iptables -P FORWARD DROP
# iptables -A INPUT -j DROP
# iptables -A OUTPUT -j DROP
# make a commandline addremove feature
iptables -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
#TELNET
iptables -A INPUT -p tcp -m tcp --dport 20 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 20 -j ACCEPT
#FTP
iptables -A INPUT -p tcp -m tcp --dport 990 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 990 -j ACCEPT
iptables -A INPUT -p tcp --dport 15000:16000 -j ACCEPT
iptables -A OUTPUT -p tcp --dport 15000:16000 -j ACCEPT
iptables -A INPUT -p udp --dport 15000:16000 -j ACCEPT
iptables -A OUTPUT -p udp --dport 15000:16000 -j ACCEPT

iptables -A INPUT -p udp -m udp --dport 21 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 21 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 21 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 21 -j ACCEPT

iptables -A INPUT -p udp -m udp --dport 69 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 69 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 69 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 69 -j ACCEPT

#SSH
#iptables -A INPUT -p tcp -m tcp --dport 22 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 22 -j ACCEPT

iptables -A OUTPUT -p tcp -m tcp --dport 23 -j ACCEPT
#
iptables -A OUTPUT -p tcp -m tcp --dport 53 -j ACCEPT
#SMTP
iptables -A INPUT -p tcp -m tcp --dport 25 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 25 -j ACCEPT
#WHOIS
iptables -A INPUT -p tcp -m tcp --dport 43 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 43 -j ACCEPT
#DNS
iptables -A INPUT -p udp -m udp --dport 53 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 53 -j ACCEPT
#HTTP
iptables -A INPUT -p tcp -m tcp --dport 80 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 80 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 8080 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 8080 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 8181 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 8181 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 2050 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 2050 -j ACCEPT
#HTTPS
iptables -A INPUT -p tcp -m tcp --dport 110 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 110 -j ACCEPT
#GIT
iptables -A INPUT -p tcp -m tcp --dport 9418 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 9418 -j ACCEPT
#EMAIL
iptables -A INPUT -p tcp -m tcp --dport 143 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 143 -j ACCEPT
#SSL WEB
iptables -A INPUT -p tcp -m tcp --dport 443 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 443 -j ACCEPT
#email - SMTP 
iptables -A INPUT -p tcp -m tcp --dport 465 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 465 -j ACCEPT
#email - imapssl
iptables -A INPUT -p tcp -m tcp --dport 995 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 995 -j ACCEPT
#email - popssl
iptables -A INPUT -p tcp -m tcp --dport 993 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 993 -j ACCEPT
#HOSTING CPANEL
iptables -A INPUT -p tcp -m tcp --dport 2083 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 2083 -j ACCEPT
#SHOUTCAST
iptables -A INPUT -p tcp -m tcp --dport 8000 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 8000 -j ACCEPT
#IRC
iptables -A INPUT -p tcp -m tcp --dport 6697 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 6697 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 6667 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 6667 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 9999 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 9999 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 6668 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 6668 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 6669 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 6669 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 7001 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 7001 -j ACCEPT
#
iptables -A INPUT -p tcp -m tcp --dport 3306 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 3306 -j ACCEPT
#PRINTER
iptables -A INPUT -p tcp -m tcp --dport 515 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 515 -j ACCEPT
#SVN
iptables -A INPUT -p tcp -m tcp --dport 3690 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 3690 -j ACCEPT
#pulseaudio
iptables -A INPUT -p tcp -m tcp --dport 5353 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 5353 -j ACCEPT

#freehosting.com cpanel
iptables -A INPUT -p tcp -m tcp --dport 2222 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 2222 -j ACCEPT

#VNC
iptables -A INPUT -p tcp -m tcp --dport 5900 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 5900 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 5901 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 5901 -j ACCEPT
#Radio Meltdown
iptables -A INPUT -p tcp -m tcp --dport 2199 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 2199 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 7714 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 7714 -j ACCEPT

#
#iptables -A INPUT -p tcp --dport 49152:65535 -j ACCEPT
#iptables -A INPUT -p tcp --dport 6881:6891 -j ACCEPT
#iptables -A OUTPUT -p tcp --dport 49152:65535 -j ACCEPT
#iptables -A OUTPUT -p tcp --dport 6881:6891 -j ACCEPT
#iptables -A INPUT -p tcp --dport 56881:56889 -j ACCEPT
#iptables -A INPUT -p udp --dport 56881:56889 -j ACCEP
#upnp
#iptables -A INPUT -m udp -p udp --dport 1900 -j ACCEPT
#iptables -A OUTPUT -m udp -p udp --dport 1900 -j ACCEPT
#iptables -A INPUT -m tcp -p tcp --dport 1900 -j ACCEPT
#iptables -A OUTPUT -m tcp -p tcp --dport 1900 -j ACCEPT



#iptables -I INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT

## place udp server stuff after these

iptables -I INPUT -p icmp -j DROP
iptables -I INPUT -p udp -j DROP
iptables -I OUTPUT -p icmp -j DROP
iptables -I OUTPUT -p udp -j DROP

#Torrent
iptables -A INPUT -p tcp -m tcp --dport 51413 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 51413 -j ACCEPT
iptables -A INPUT -p udp -m udp --dport 51413 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 51413 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 58846 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 58846 -j ACCEPT
iptables -A INPUT -p udp -m udp --dport 58846 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 58846 -j ACCEPT

iptables -A INPUT -p tcp -m tcp --dport 6881 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 6881 -j ACCEPT

iptables -A INPUT -p udp -m udp --dport 8881 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 8881 -j ACCEPT


#RDP
iptables -A INPUT -p tcp -m tcp --dport 3389 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 3389 -j ACCEPT
iptables -A INPUT -p udp -m udp --dport 3389 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 3389 -j ACCEPT

#synergy
iptables -A INPUT -p tcp --dport 24800 -j ACCEPT
iptables -A OUTPUT -p tcp --dport 24800 -j ACCEPT
iptables -A INPUT -p udp -m udp --dport 24800 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 24800 -j ACCEPT
#iptables -A INPUT -p tcp --dport 8081 -j ACCEPT
#iptables -A OUTPUT -p tcp --dport 8081 -j ACCEPT
#iptables -A INPUT -p udp --dport 8081 -j ACCEPT
#iptables -A OUTPUT -p udp --dport 8081 -j ACCEPT
#iptables -A INPUT -p tcp -m tcp --dport 24800 -j ACCEPT
#iptables -A INPUT -p udp -m udp --dport 24800 -j ACCEPT

#WOL
iptables -A INPUT -p udp -m udp --dport 7 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 7 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 7 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 7 -j ACCEPT
iptables -A INPUT -p udp -m udp --dport 9 -j ACCEPT
iptables -A OUTPUT -p udp -m udp --dport 9 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --dport 9 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 9 -j ACCEPT


# allow ICMP Type 8 (ping, ICMP traceroute)
iptables -A INPUT -p icmp --icmp-type 8 -j ACCEPT
# enable UDP traceroute rejections to get sent out
iptables -A INPUT -p udp --dport 33434:33523 -j REJECT
iptables -A OUTPUT -p udp --dport 33434:33523 -j ACCEPT



# reject (not drop) packets for inbound traceroutes from Linux boxes
iptables -I INPUT -p udp --dport 33434:33474 -j REJECT

# accept ping requests for Windows-style traceroutes
iptables -I INPUT -p ICMP --icmp-type echo-request -j ACCEPT

# allow ping responses for Windows-style traceroutes
iptables -I OUTPUT -p ICMP --icmp-type echo-reply -j ACCEPT

# allow the server to perform its own traceroutes
iptables -I OUTPUT -p udp --dport 33434:33474 -j ACCEPT




iptables -I INPUT -p udp -j ACCEPT
iptables -I OUTPUT -p udp -j ACCEPT

iptables -A INPUT -i lo -j ACCEPT
iptables -A OUTPUT -o lo -j ACCEPT

iptables -A OUTPUT -j LOG
iptables -A INPUT -j LOG
iptables -A FORWARD -j LOG
 
