//https://learn.microsoft.com/de-de/dotnet/api/system.net.sockets.udpclient.joinmulticastgroup?view=net-7.0
//https://learn.microsoft.com/de-de/dotnet/api/system.net.sockets.multicastoption?view=net-8.0

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class MulticastReceiver {
    static void Main() {

        IPAddress multicastGroupIP = IPAddress.Parse("232.0.0.0"); 
        int multicastPort = 1900; 

        // create UDP socket for multicast
        Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        
        // reuse address
        s.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);

        // bind socket IPAddress.Any can be replaced with local IP
        EndPoint localEP = (EndPoint)new IPEndPoint(IPAddress.Any, multicastPort);
        s.Bind(localEP);

        //join multicast group (ASM)
        MulticastOption mcastOption = new MulticastOption(multicastGroupIP, IPAddress.Any);
        s.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, mcastOption);

        // receiving
        byte[] bytes = new Byte[1024];
        EndPoint remoteEP = (EndPoint)new IPEndPoint(IPAddress.Any, 0);
        while (true) {
            // Receive the message.
            int nbytes = s.ReceiveFrom(bytes, ref remoteEP);

            // Convert the byte array to a string.
            string receivedData = Encoding.ASCII.GetString(bytes, 0, nbytes);

            // Display message
            Console.WriteLine(receivedData);
        }
        s.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.DropMembership, mcastOption);
        s.Close();
    }
}