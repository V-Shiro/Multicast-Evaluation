// Source: https://learn.microsoft.com/de-de/dotnet/api/system.net.sockets.multicastoption?view=net-8.0
// Source: https://learn.microsoft.com/de-de/dotnet/api/system.net.sockets.udpclient.joinmulticastgroup?view=net-7.0
// Source: https://www.webdevtutor.net/blog/c-sharp-multicast-socket-example
// Source: https://stackoverflow.com/questions/5145804/how-to-join-source-specific-multicast-group-in-c-sharp-igmpv3

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class MulticastReceiver {
    static void Main() {
            //IPv6 IP
            IPAddress multicastGroupIP = IPAddress.Parse("ff02::1"); 
            int multicastPort = 2000; 

            // create UDP socket for multicast with IPv6
            Socket s = new Socket(AddressFamily.InterNetworkV6, SocketType.Dgram, ProtocolType.Udp);
            
            // bind socket IPAddress.Any can be replaced with local IP
            EndPoint localEP = (EndPoint)new IPEndPoint(IPAddress.IPv6Any, multicastPort);
            s.Bind(localEP);

            //join multicast group (ASM) with IPv6
            IPv6MulticastOption mcastOption = new IPv6MulticastOption(multicastGroupIP);
            s.SetSocketOption(SocketOptionLevel.IPv6, SocketOptionName.AddMembership, mcastOption);
   
            // receiving
            byte[] bytes = new Byte[1024];
            //IPEndPoint groupEP = new(multicastGroupIP, multicastPort);
            EndPoint remoteEP = (EndPoint)new IPEndPoint(IPAddress.Any, 0);
            while (true) {
                // Receive the message.
                int nbytes = s.ReceiveFrom(bytes, ref remoteEP);

                // Convert the byte array to a string.
                string receivedData = Encoding.ASCII.GetString(bytes, 0, nbytes);

                // Display message
                Console.WriteLine(receivedData);
            }
            s.Close();
    }
}