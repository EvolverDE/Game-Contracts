using System;
using System.Collections.Generic;

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
    public List<bool> Votes { get; set; } = new List<bool>();
}

public class MapController
{
    private Dictionary<string, Poll> polls = new Dictionary<string, Poll>();
    private int currentProviderIndex = 0;
    private int currentElectionIndex = 0;

    public void ProcessMapEntries(List<MapEntry> entries)
    {
        foreach (var entry in entries)
        {
            switch (entry.Key1)
            {
                case 1:
                    HandleProviderEntry(entry);
                    break;
                case 2:
                    HandleEntitledEntry(entry);
                    break;
                case 3:
                    HandleElectionEntry(entry);
                    break;
                case 4:
                    HandleProviderIDEntry(entry);
                    break;
                case 5:
                    HandleActorIDEntry(entry);
                    break;
                case 6:
                    HandleTargetIDEntry(entry);
                    break;
                case 7:
                    HandleMainMethodEntry(entry);
                    break;
                case 8:
                    HandleSubMethodEntry(entry);
                    break;
                case 9:
                    HandleParameterEntry(entry);
                    break;
                case 10:
                    HandleParameter2OrTimeoutEntry(entry);
                    break;
                case 11:
                    HandleAgreedersEntry(entry);
                    break;
                case 12:
                    HandleRejectersEntry(entry);
                    break;
                default:
                    HandleVoteEntry(entry);
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
            if (pol.Timeout > DateTime.Now && pol.IsEntitled)
            {
                activePolls.Add(poll);
            }
        }
        return activePolls;
    }
}
