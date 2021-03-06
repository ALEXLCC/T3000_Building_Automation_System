﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.OleDb;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections; 


namespace WFA_psychometric_chart
{
    public partial class form_app_timer : Form
    {
        private Form1 form1;
        public form_app_timer(Form1 form1)
        {
            this.form1 = form1;
            InitializeComponent();
        }

        int first_enable = 0;
        int second_enable = 0;
        ArrayList new_checked_item_index = new ArrayList();
        private void form_app_timer_Load(object sender, EventArgs e)
        {
            //btn_from_month_down.Text = char.ConvertFromUtf32(8595);
            
            //dtp1.Value = new DateTime(DateTime.Now.Year, 1, 1);
            //dtp2.Value = new DateTime(DateTime.Now.Year,DateTime.Now.Month,DateTime.Now.Day);
            //dateTimePicker.MinDate = DateTime.Now;
           // dateTimePicker.MaxDate = DateTime.Now.AddDays(15);
            dtp1.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp2.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp1.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp2.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //when this one is clicked 
            //mc1.Visible = true;

            
        }

        private void monthCalendar1_DateChanged(object sender, DateRangeEventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {

           // mc2.Visible = true;
        }

        public class data_type_hum_temp
        {
            public double temp { get; set; }
            public double hum { get; set; }
            public string  date { get; set; }
            public int hour { get; set; }
        }
        List<data_type_hum_temp> hist_temp_hum_list = new List<data_type_hum_temp>();
        List<data_type_hum_temp> temp_hist_temp_hum_list = new List<data_type_hum_temp>(); 

        //this is the function that plotes the values form the graph..
        public void plot_on_first_graph()
        {
             


            if (first_enable == 1 && second_enable == 1)
            {

                if (checkedListBox1.CheckedItems.Count > 0)
                {

                string t = null;

                
                //lets check if the hour was changed or not...
                foreach(object item in checkedListBox1.CheckedItems)//this is done like this because the items checked has discarded in 0.                
                {
                    int index = checkedListBox1.Items.IndexOf(item);
                    if (index != 0) { 
                    new_checked_item_index.Add(index);
                    //t += index + ",\t";
                    }
                   


                }


                    //then only do the changes else we dont have to do it.
                    //MessageBox.Show("it works\n "+t);
                   
                //lets do the plotting part here..

                  if (dtp2.Value > dtp1.Value) { 

                  string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                  string connString1 = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";

                  using (OleDbConnection connection1 = new OleDbConnection(connString1))
                  {
                      connection1.Open();



                      //string sql_query = "Select * from tbl_data_stored_temp_hum_one_year WHERE date_current = " + day_list[i] + " , hour_current = " + hour_al[h] + " AND station_name = "+ station_name +" ; ";
                      //lets pass this string to a query which does the pulling part.
                      OleDbDataReader reader1 = null;
                      OleDbCommand command1 = new OleDbCommand("Select * from tbl_historical_data WHERE date_current BETWEEN @date_first AND @date_second", connection1);
                      command1.Parameters.AddWithValue("@date_first", dtp1.Value);
                      command1.Parameters.AddWithValue("@date_second", dtp2.Value);
                      //command1.Parameters.AddWithValue("@station_name", station_name);
                      reader1 = command1.ExecuteReader();
                      while (reader1.Read())
                      {
                          //station_name = reader["station_name"].ToString();
                          hist_temp_hum_list.Add(
                              new data_type_hum_temp
                              {
                                  temp = double.Parse(reader1["temperature"].ToString()),
                                  hum = double.Parse(reader1["humidity"].ToString()),
                                  date = reader1["date_current"].ToString(),
                                  hour = int.Parse(reader1["hour_current"].ToString())
                              });
                      }//close of while loop       
                      // connection1.Close();
                  }//close of database using statement 

                      //lets display the data 
                  //string test = null;
                  //for (int i = 0; i < hist_temp_hum_list.Count; i++)
                  //{
                  //    test += hist_temp_hum_list[i].temp+" , hum=  "+ hist_temp_hum_list[i].hum +",date="+hist_temp_hum_list[i].date+",hour="+hist_temp_hum_list[i].hour+"\n";
                    
                  //}
                  //MessageBox.Show("value = \n"+test);
                //since we have the data now we can start plotting it in the form..
                      for(int i=0;i < new_checked_item_index.Count ; i++)
                      {
                          for (int y = 0; y < hist_temp_hum_list.Count;y++ )
                          {
                             //here we compare the desired outcome...
                              if (new_checked_item_index[i].ToString() ==hist_temp_hum_list[y].hour.ToString())
                              {
                                  //then push it into the new list..
                                  temp_hist_temp_hum_list.Add(hist_temp_hum_list[y]);
                                  hist_temp_hum_list.RemoveAt(y);
                              }
                          }
                      }//close of for..

                      string test = null;
                      for (int i = 0; i < temp_hist_temp_hum_list.Count; i++)
                      {
                          test += temp_hist_temp_hum_list[i].temp + " , hum=  " + temp_hist_temp_hum_list[i].hum + ",date=" + temp_hist_temp_hum_list[i].date + ",hour=" + temp_hist_temp_hum_list[i].hour + "\n";

                      }
                      MessageBox.Show("after filtering the values of hour = \n" + test);
                    //now lets plot it in the graph
                      for (int x = 0; x < temp_hist_temp_hum_list.Count; x++)
                      {
                          //this plots the value
                          form1.plot_by_DBT_HR(temp_hist_temp_hum_list[x].temp, temp_hist_temp_hum_list[x].hum / 100);
                        
                      }
                      //now lets reset all the values..
                      temp_hist_temp_hum_list.Clear();
                      hist_temp_hum_list.Clear();
                      new_checked_item_index.Clear();



                  }//close of comparision if
                  else
                  {
                      MessageBox.Show("Please enter a valid dates,Date chosen in 'From' section should be smaller than that in 'To' section");
                  }
                
                  

                }//close of if checkedItem>0
                else
                {
                    MessageBox.Show("Please Select one or more hours first");
                }

            }//close of if
            
        }//close of the main function .

        private void dtp1_event_onValue_Change(object sender, EventArgs e)
        {
            //lets enable the event and see what happens ..
            first_enable = 1;//event enabler , this is done to see if the value is changed or not if both the value of To and From
                             //is changed then we make the calculation..

            plot_on_first_graph();


        }

        private void dtp2_event_onValue_Change(object sender, EventArgs e)
        {
            //this value for second one ...
            second_enable = 1;//enable the event 
           // dtp1.MaxDate = dtp2.Value;//this is done so that the maximun value changed of the second is max for 1 compulasary.
            plot_on_first_graph();
        }
       // int item_selected = 0;
        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

            if (checkedListBox1.SelectedItem == checkedListBox1.Items[0])
            {
                //if(checkedListBox1.SelectedIndex==0)
                if (checkedListBox1.GetItemChecked(0) == true)
                {

                    //then checked the items..
                    //MessageBox.Show("item checked is" + checkedListBox1.SelectedIndex);
                    for (int i = 1; i <= 24; i++)
                    {
                        checkedListBox1.SetItemChecked(i, true);

                    }


                }//close of ir
                else if (checkedListBox1.GetItemChecked(0) == false)
                {
                    for (int i = 1; i <= 24; i++)
                    {
                        checkedListBox1.SetItemChecked(i, false);

                    }

                }




            }
        }//close of flb.selecteditem




    }
}
