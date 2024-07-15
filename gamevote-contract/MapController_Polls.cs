using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapController_Polls
{
    public class MapEntry
    {
        public int Key1 { get; set; }
        public int Key2 { get; set; }
        public long Value { get; set; }
    }

    public class Poll
    {
        public int ProviderID { get; set; }
        public long AmountNQT { get; set; }
        public bool IsEntitled { get; set; }
        public string HashValue { get; set; }
        public long ActorID { get; set; }
        public long TargetID { get; set; }
        public string MainMethod { get; set; }
        public string SubMethod { get; set; }
        public long Parameter { get; set; }
        public long Parameter2 { get; set; }
        public DateTime Timeout { get; set; }
        public int AgreedersCount { get; set; }
        public int RejectersCount { get; set; }
    }

    public class MapController
    {
        private Dictionary<string, Poll> polls = new Dictionary<string, Poll>();
        private int providerIdCounter = 0;
        private int electionCounter = 0;

        public void ProcessMapEntries(List<MapEntry> entries)
        {
            foreach (var entry in entries)
            {
                switch (entry.Key1)
                {
                    case 1:
                        ProcessProvider(entry);
                        break;
                    case 2:
                        ProcessEntitlement(entry);
                        break;
                    case 3:
                        ProcessElection(entry);
                        break;
                    case 4:
                        StoreProviderID(entry);
                        break;
                    case 5:
                        StoreActorID(entry);
                        break;
                    case 6:
                        StoreTargetID(entry);
                        break;
                    case 7:
                        StoreMainMethod(entry);
                        break;
                    case 8:
                        StoreSubMethod(entry);
                        break;
                    case 9:
                        StoreParameter(entry);
                        break;
                    case 10:
                        StoreParameter2(entry);
                        break;
                    case 11:
                        StoreAgreeders(entry);
                        break;
                    case 12:
                        StoreRejecters(entry);
                        break;
                    default:
                        break;
                }
            }
        }

        private void ProcessProvider(MapEntry entry)
        {
            if (entry.Key2 == 0)
            {
                providerIdCounter++;
            }
            else
            {
                var hashValue = entry.Value.ToString();
                if (!polls.ContainsKey(hashValue))
                {
                    polls[hashValue] = new Poll();
                }
                polls[hashValue].ProviderID = (int)entry.Value;
            }
        }


    }
