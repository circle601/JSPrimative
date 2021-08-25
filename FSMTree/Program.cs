using FSMTree.StateMachine;
using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.IO;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;

namespace FSMTree
{




    class Program
    {


        static WordTree Tree = new WordTree();

        public static void Addword(string Word)
        {
            WordTree TreeItem = Tree;
            foreach (char Letter in Word)
            {
                if (TreeItem.Children == null) TreeItem.Children = new Dictionary<char, WordTree>();
                WordTree NewChild;
                if (!TreeItem.Children.ContainsKey(Letter))
                {
                    TreeItem.Children[Letter] = new WordTree();
                }
                NewChild = TreeItem.Children[Letter];
                NewChild.Letter = Letter;



                TreeItem = NewChild;


            }
            TreeItem.Valid = true;
            TreeItem.Word = Word;
        }


        static string BuildTree(WordTree treeItem)
        {
            if(treeItem.Children == null)
            {
                if (!treeItem.Valid) throw new Exception();
                return "" + treeItem.Letter.ToString();
            }
            else
            {
                StringBuilder Sb = new StringBuilder();
                bool First = true;
                if (treeItem.Letter != 0)
                {
                    Sb.Append(treeItem.Letter.ToString());
                }
                if (treeItem.Children.Count > 1) Sb.Append("(");
                foreach (var item in treeItem.Children)
                {
                    if (!First) Sb.Append("|");
                    First = false;
                    Sb.Append(BuildTree(item.Value));
                }
                if (treeItem.Children.Count > 1) Sb.Append(")");
                return Sb.ToString();
            }
        }




        static Dictionary<int, WordTree> StateDict = new Dictionary<int, WordTree>();
        static char Index = (char)0;

        public static void BuildState(WordTree treeItem)
        {
            StateDict.Add(Index, treeItem);
            treeItem.StateNumber = Index++;

            if (treeItem.Children != null)
            {
                foreach (var item in treeItem.Children)
                {
                    BuildState(item.Value);
                }
            }
        }


       static char[,] Table;

        public static void BuildTable(WordTree treeItem)
        {
            int Index = treeItem.StateNumber;

            if (treeItem.Children != null)
            {
                for (char letter = 'a'; letter <= 'z'; letter++)
                {
                    if (treeItem.Children.ContainsKey(letter))
                    {

                        Table[Index, letter - 'a'] = treeItem.Children[letter].StateNumber;
                    }
                    else
                    {
                        Table[Index,letter - 'a'] = (char)0;
                    }
                }

                foreach (var item in treeItem.Children)
                {
                    BuildTable(item.Value);
                }
            }
            else
            {
                for (char letter = 'a'; letter <= 'z'; letter++)
                {
                    Table[Index, letter - 'a'] = (char)0;
                }
            }
        }



        public static string BuildValid()
        {
            bool[] Result = new bool[256];
            foreach (var item in StateDict)
            {
                Result[item.Key] = item.Value.Valid;
            }

            StringBuilder Datastring = new StringBuilder();
            Datastring.Append("const char* ResultTable = \"");

                for (int i = 0; i < 256; i++)
                {

                if (Result[i])
                {
                    Datastring.Append("b");
                }
                else{
                    Datastring.Append("a");
                }
             

            }
            Datastring.AppendLine("\";");
            return Datastring.ToString();


            }



        public static string BuildFSM()
        {
            BuildState(Tree);

            foreach (var item in StateDict)
            {
                Console.WriteLine(item.Key + ":" + item.Value.Letter);
            }


            Table = new char[Index,26 ];

            BuildTable(Tree);

            var Datastring = new StringBuilder();




            Datastring.Append("const char* LookupTable = \"");

            if (false)
            {



                for (int let = 0; let < 26; let++)
                {
                    for (int i = 0; i < 256; i++)
                    {


                        if (i >= Index)
                        {
                            Datastring.Append("\0");
                        }
                        else
                        {
                            char Item = Table[i, let];
                            if (Item != '"' && Item != '\'' && Item != '?' && Item != '\\' && (char.IsLetterOrDigit(Item) || Char.IsPunctuation(Item) || char.IsSymbol(Item) || Item == ' '))
                            {
                                Datastring.Append(Item);
                            }
                            else if (Item == 0)
                            {
                                Datastring.Append(@"\0");
                            }
                            else if (Item == '?')
                            {
                                Datastring.Append(@"\?");
                            }
                            else if (Item == '\\')
                            {
                                Datastring.Append(@"\\");
                            }
                            else if (Item == '\'')
                            {
                                Datastring.Append(@"\'");
                            }
                            else if (Item == '\"')
                            {
                                Datastring.Append("\\\"");
                            }
                            else
                            {
                                Datastring.Append("\\x");
                                Datastring.Append(Convert.ToByte(Item).ToString("x2"));
                            }
                        }
                    }
                }
                Datastring.AppendLine("\";");


            }
            else
            {
                Datastring.AppendLine("{");
                bool first = true;
                for (int let = 0; let < 26; let++)
                {
                    for (int i = 0; i < 256; i++)
                    {
                        
                        if (i >= Index)
                        {
                            Datastring.Append(",0");
                        }
                        else {
                            char Item = Table[i, let];
                            if (!first)
                            {
                                Datastring.Append(",");
                            }
                            first = false;
                            Datastring.Append((int)Item);
                        }

                    }
                }
                Datastring.AppendLine("};");
            }
            string DatastringR = Datastring.ToString();

          






            var csv = new StringBuilder();
            //csv.Append("State , ");
            for (int y = 0; y < 26; y++)
            {
                csv.Append((char)('a' + y));
                csv.Append(" , ");

            }
            csv.AppendLine();

            for (int x = 0; x < Index; x++)
            {
                //csv.Append("state ");
                //csv.Append(x);
                //csv.Append(" ,");

                for (int y = 0; y < 26; y++)
                {
                    csv.Append((int)Table[x, y]);
                    csv.Append(" , ");
                    
                }
                csv.AppendLine();
            }

            string result = csv.ToString();
            File.WriteAllText(@"C:\Users\Northern Lights\Desktop\Projects\Js\JSReduced.csv", result);
            return Datastring.ToString();
        }


        static void Main(string[] args)
        {


            string Code = "Hello ::= test 'test' test ;";



            using(FileStream File = new FileStream(@"C:\OldhardDisk\VisualStudio\source\repos\JSPrimative\FSMTree\BNF Resources\js.bnf", FileMode.Open)){
                BNFParser parser = new BNFParser(File);
                parser.ParseLine();

            }
       





            string[] Lines = File.ReadAllLines(@"C:\Users\Northern Lights\Desktop\Projects\Js\JSReduced.txt");


           var Letters = Lines.SelectMany((x) => x.Trim().ToLowerInvariant().ToCharArray()).Distinct().ToImmutableSortedSet();




           // Addword("var".Trim().ToLowerInvariant());

              foreach (var item in Lines)
              {
            
                 Addword(item.Trim().ToLowerInvariant());
             }
            string result = BuildTree(Tree);
          
            string Result = BuildFSM()   + "\n" + BuildValid();
            Console.WriteLine(result);
            Console.WriteLine("Hello World!");
        }
    }
}
