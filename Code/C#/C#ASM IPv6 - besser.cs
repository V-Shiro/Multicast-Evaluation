// Source: https://learn.microsoft.com/de-de/dotnet/api/system.net.sockets.multicastoption?view=net-8.0
// Source: https://learn.microsoft.com/de-de/dotnet/api/system.net.sockets.udpclient.joinmulticastgroup?view=net-7.0
// Source: https://www.webdevtutor.net/blog/c-sharp-multicast-socket-example

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
        UdpClient s = new UdpClient(AddressFamily.InterNetworkV6);

        // reuse address / port
        s.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);

        // bind socket IPAddress.Any can be replaced with local IP
        EndPoint localEP = (EndPoint)new IPEndPoint(IPAddress.IPv6Any, multicastPort);
        s.Client.Bind(localEP);

        //join multicast group (ASM) with IPv6
        s.JoinMulticastGroup(multicastGroupIP);

        // receiving
        byte[] bytes = new Byte[1024];
        IPEndPoint remoteEP = new IPEndPoint(IPAddress.IPv6Any, 0);
        while (true) {
            // Receive the message.
            bytes = s.Receive(ref remoteEP);

            // Convert the byte array to a string.
            string receivedData = Encoding.ASCII.GetString(bytes);

            // Display message
            Console.WriteLine(receivedData);
        }
            s.Close();
    }
}