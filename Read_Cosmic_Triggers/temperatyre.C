// Try to plot hit rate per minute vs temperature
{
  TProfile *h = new TProfile("temperature","Hit Rate per Minute vs Temperature;Temperature (degree F);Hit Rate per Minute",30,70,85,50,150);
  int N=0;

  int evtnum;
  unsigned int tim;
  float temp, humidity;
  
  t->SetBranchAddress("evtnum",&evtnum);
  t->SetBranchAddress("time",&tim);
  t->SetBranchAddress("temp",&temp);
  t->SetBranchAddress("humidity",&humidity);

  t->GetEntry(0);
  unsigned int t0 = tim;
  float tem=0;
  for (int i=1; i<t->GetEntries(); ++i) {
    t->GetEntry(i);
    if (tim-t0<60) {
      ++N;
      tem += temp;
    }
    else {
      tem /= N;
      h->Fill(tem, N);
      N = 0;
      tem = 0;
      t0 = tim;
    }
  }
}
