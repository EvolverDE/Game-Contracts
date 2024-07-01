using System;

namespace PersoChecker
{
    class Program
    {
        static void Main(string[] args)
        {
            while (true)
            {
                Console.WriteLine("Bitte gib die Personalausweisnummer ein (Format: IDA IDB IDC IDD):");
                string persoId = Console.ReadLine();

                if (PersoChecksum(persoId))
                {
                    Console.WriteLine("Personalausweisnummer korrekt!");

                    if (!PersoGueltig(persoId))
                    {
                        Console.WriteLine("Perso ist abgelaufen!");
                    }

                    var data = PersoInfo(persoId);
                    Console.WriteLine("\nDaten der Ausweisnummer:");
                    Console.WriteLine($"Alter: {data.Alter}");
                    Console.WriteLine($"Geburtsmonat: {data.Geburtsmonat}");
                    Console.WriteLine($"Ablauftag: {data.AblaufTag}");
                }
                else
                {
                    Console.WriteLine("Personalausweisnummer falsch!");
                }

                Console.WriteLine("\nMöchtest du eine weitere Nummer überprüfen? (j/n):");
                if (Console.ReadLine().ToLower() != "j")
                {
                    break;
                }
            }
        }

        static bool CheckNumber(string id, char checkNumber)
        {
            int[] weights = { 7, 3, 1 };
            int sum = 0;
            int weightIndex = 0;

            foreach (char c in id)
            {
                int value = char.IsDigit(c) ? c - '0' : c - 'A' + 10;
                sum += value * weights[weightIndex];
                weightIndex = (weightIndex + 1) % weights.Length;
            }

            char lastDigit = sum.ToString().Last();
            return lastDigit == checkNumber;
        }

        static string PersoType(string id)
        {
            string[] splits = id.ToUpper().Split(' ');

            if (splits[0].Length == 11 && splits[1].Length == 7 && splits[2].Length == 7 && splits[3].Length == 1)
            {
                return "old";
            }
            else if (splits[0].Length == 10 && splits[1].Length == 7 && splits[2].Length == 8 && splits[3].Length == 1)
            {
                return "new";
            }
            else
            {
                return "unknown";
            }
        }

        static bool PersoChecksum(string id)
        {
            string[] splits = id.ToUpper().Split(' ');
            string persoType = PersoType(id);

            if (persoType == "unknown")
            {
                return false;
            }

            string[] checksums = {
                splits[0].Substring(0, 9) + splits[0][9],
                splits[1].Substring(0, 6) + splits[1][6],
                splits[2].Substring(0, 6) + splits[2][6],
                splits[0].Substring(0, 10) + splits[1].Substring(0, 7) + splits[2].Substring(0, 7) + splits[3]
            };

            foreach (string checksum in checksums)
            {
                if (!CheckNumber(checksum.Substring(0, checksum.Length - 1), checksum.Last()))
                {
                    return false;
                }
            }

            return true;
        }

        static bool PersoGueltig(string id)
        {
            string[] splits = id.Split(' ');
            DateTime validUntil = new DateTime(2000 + int.Parse(splits[2].Substring(0, 2)),
                                              int.Parse(splits[2].Substring(2, 2)),
                                              int.Parse(splits[2].Substring(4, 2)));

            return DateTime.Now <= validUntil;
        }

        static dynamic PersoInfo(string id)
        {
            string[] splits = id.Split(' ');
            dynamic info = new System.Dynamic.ExpandoObject();
            info.PersoType = PersoType(id);

            info.Geburtstag = splits[1].Substring(4, 2);
            info.Geburtsmonat = splits[1].Substring(2, 2);
            info.Geburtsjahr = splits[1].Substring(0, 2);

            if (int.Parse(info.Geburtsjahr) > int.Parse(DateTime.Now.ToString("yy")))
            {
                info.Geburtsjahr = "19" + info.Geburtsjahr;
            }
            else
            {
                info.Geburtsjahr = "20" + info.Geburtsjahr;
            }

            int alter = DateTime.Now.Year - int.Parse(info.Geburtsjahr);

            if (DateTime.Now.Month < int.Parse(info.Geburtsmonat) ||
                (DateTime.Now.Month == int.Parse(info.Geburtsmonat) && DateTime.Now.Day < int.Parse(info.Geburtstag)))
            {
                alter--;
            }

            info.Alter = alter;
            info.Volljaehrig = alter >= 18;

            info.AblaufTag = splits[2].Substring(4, 2);
            info.AblaufMonat = splits[2].Substring(2, 2);
            info.AblaufJahr = "20" + splits[2].Substring(0, 2);

            if (info.PersoType == "old")
            {
                info.Herkunft = splits[0][10];
            }
            else
            {
                info.Herkunft = splits[2][7];
            }

            info.Deutscher = char.ToLower(info.Herkunft) == 'd';
            info.Behoerdenkennzahl = splits[0].Substring(0, 4);

            return info;
        }
    }
}
