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

        private void HandleProviderEntry(MapEntry entry)
        {
            if (entry.Key2 == 0)
            {
                currentProviderIndex++;
            }
            else if (entry.Key2 == 1)
            {

                var providerID = (int)(entry.Value >> 32);
                var amountNQT = entry.Value & 0xFFFFFFFF;
                if (!polls.ContainsKey(providerID.ToString()))

                {
                    polls[providerID.ToString()] = new Poll { ProviderID = providerID, AmountNQT = amountNQT };
                }
            }
        }

        private void HandleEntitledEntry(MapEntry entry)
        {
            if (entry.Key2 == 0) return;
            var providerID = (int)entry.Value;
            if (polls.ContainsKey(providerID.ToString()))
            {
                polls[providerID.ToString()].IsEntitled = entry.Value != 0;
            }
        }

        private void HandleElectionEntry(MapEntry entry)
        {
            if (entry.Key2 == 0)
            {
                currentElectionIndex++;
            }
            else if (entry.Key2 == 1)
            {

                var hashValue = entry.Value.ToString("X");
                if (!polls.ContainsKey(hashValue))

                {
                    polls[hashValue] = new Poll { HashValue = hashValue };
                }
            }
        }

        private void HandleProviderIDEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].ProviderID = (int)entry.Value;
            }
        }

        private void HandleActorIDEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].ActorID = entry.Value;
            }
        }

        private void HandleTargetIDEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].TargetID = entry.Value;
            }
        }

        private void HandleMainMethodEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].MainMethod = entry.Value.ToString();
            }
        }

        private void HandleSubMethodEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].SubMethod = entry.Value.ToString();
            }
        }

        private void HandleParameterEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].Parameter = entry.Value;
            }
        }

        private void HandleParameter2OrTimeoutEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                if (entry.Key2 == 9)
                {
                    polls[hashValue].Parameter2 = entry.Value;
                }
                else if (entry.Key2 == 10)
                {
                    polls[hashValue].Timeout = DateTimeOffset.FromUnixTimeSeconds(entry.Value).DateTime;
                }
            }
        }

        private void HandleAgreedersEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].AgreedersCount = (int)entry.Value;
            }
        }

        private void HandleRejectersEntry(MapEntry entry)
        {
            var hashValue = entry.Key2.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].RejectersCount = (int)entry.Value;
            }
        }

        private void HandleVoteEntry(MapEntry entry)
        {
            var hashValue = entry.Key1.ToString("X");
            if (polls.ContainsKey(hashValue))
            {
                polls[hashValue].Votes.Add(entry.Value != 0);
            }
        }

        public List<Poll> GetActivePolls()
        {
            var activePolls = new List<Poll>();
            foreach (var poll in polls.Values)
            {
                if (poll.Timeout > DateTime.Now && poll.IsEntitled)
                {
                    activePolls.Add(poll);
                }
            }
            return activePolls;
        }
    }

}
