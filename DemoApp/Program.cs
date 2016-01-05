using System;
using System.Threading;

namespace DemoApp
{
	class Program
	{
		static void Main(string[] args)
		{
			while (!Console.KeyAvailable || (Console.KeyAvailable && Console.ReadKey(true).Key != ConsoleKey.Q))
			{
				One();
				Thread.Sleep(1000);
			}
		}

		private static void One()
		{
			bool flag = false;
			if (!flag)
			{
				Console.WriteLine("false");
			}
			else
			{
				Console.WriteLine("true");
			}
		}
	}
}
