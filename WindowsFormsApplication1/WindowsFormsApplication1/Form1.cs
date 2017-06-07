using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        private bool bComplete;
        private WebBrowser wbSearch;

        public Form1()
        {
            InitializeComponent();

            wbSearch = new WebBrowser();
            wbSearch.AllowWebBrowserDrop = false;
            wbSearch.ScriptErrorsSuppressed = true;
            wbSearch.DocumentCompleted += WbSearch_DocumentCompleted;           
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = "naver.com";
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            timer1.Stop();
        }
                
        private void WbSearch_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            if (bComplete) return;

            string sOuterHtml = wbSearch.Document.Body.Parent.OuterHtml;
            int nIndex = sOuterHtml.IndexOf("<DIV id=time_area");
            if (nIndex > 0)
            {
                bComplete = true;
                string sText = sOuterHtml.Substring(nIndex + 1, 63);
                if (sText != "")
                {
                    nIndex = sText.IndexOf(">");
                    if (nIndex > 0)
                    {
                        string sTime = sText.Substring(nIndex + 1, sText.Length - nIndex - 1);
                        textBox2.Text = "";
                        textBox2.Text = sTime;
                    }
                }
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (bComplete)
            {
                timer1.Stop();
                timer1.Start();
                InitWebLoad();
            }
        }

        private void InitWebLoad()
        {
            bComplete = false;
            string sUrl = string.Format("https://time.navyism.com/?host={0}", textBox1.Text);
            wbSearch.Navigate(sUrl);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "") return;

            timer1.Stop();
            timer1.Start();
            InitWebLoad();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
