/*
this class is for reporting the result of code with conditions
 content of csv is 
 - size of one module / screen size
 - number of module
 - population density
 - counts of detection
 - loop timing range
 - blinking time length
 
 what I really need:
 - population density
 - filled area (%)
 - detect count / average loop time
 - average of loop time
 - blink time


 another csv (will be seond try)
 - timing of blinking
 (gonna be heavy...?)
 
 */
void report() {
  //number of module
  float nom = row*column;
  //screen size
  float ss = height*width;
  //population density (how many mods on 100*100)
  float pd = nom / (ss/10000);
  //filled area (%)
  float fa =  (moduleSize*moduleSize*3.14)*nom / ss * 100;
  //counts of detection
  for (Rotator r : rots) {
      detectCount = detectCount + r.detectCount;
    }
  int dt = detectCount;
  //average of loop time
  for (Rotator r : rots) {
      totalLT = totalLT + r.loopTime;
    }
  float avlt = totalLT / nom;
  //detect count / average loop time
  float dtpavlt = dt / (reportTime / avlt);
  //blinking time length
  int btl = blinkTime;

  output.println(pd + "," + fa + "," + dtpavlt + "," + avlt + "," + btl);
  // output.println(somss);
  // output.println(nom);
  // output.println(pd);
  // output.println(dt);
  // output.println(dtpm);
  // output.println(ltrh);
  // output.println(ltrl);
  // output.println(btl);

  output.flush();
  output.close();
  exit();
}