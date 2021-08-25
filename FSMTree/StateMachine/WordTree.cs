using System;
using System.Collections.Generic;
using System.Text;

namespace FSMTree.StateMachine
{
    class WordTree
    {
        public char Letter;
        public Dictionary<char, WordTree> Children;
        public string Word;
        public bool Valid;
        public char StateNumber;
    }
}
