using System;
using System.Collections.Generic;
using System.IO;
using NUnit;


class Program
{
    static void Main(string[] args)
    {
        //Pfad
        string filePath = "C:/unittest.csv";

        //einlesen
        var entries = CsvReader.ReadCsv(filePath);

        //einträge verarbeiten
        var mapController = new MapController();
        mapController.ProcessMapEntries(entries);

        //polls abrufen/anzeigen
        var activePolls = mapController.GetActivePolls();
        Console.WriteLine("Activ Polls");
        foreach (var pol in activePolls)
        {
            Console.WriteLine($"ProviderID: {poll.ProviderID}, AmountNQT: {poll.AmountNQT}, IsEntitled: {poll.IsEntitled}, Timeout: {poll.Timeout}");
        }
    }
}

