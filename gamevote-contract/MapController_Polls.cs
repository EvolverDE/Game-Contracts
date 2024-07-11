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

    }

}
