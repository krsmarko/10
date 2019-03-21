#include "TXLib.h"
#include <queue>
#include <sstream>
#include <iostream>



struct VoidButton
{

 int mouseposx;
 int mouseposy;


 int x, y;
 int sizex, sizey;


 VoidButton              (int x_, int y_, int sizex_, int sizey_);


 virtual void DrawButton ();
 virtual void Action     ();
 virtual int  InsideTest ();
};

struct ButtonManager
{
 int lastpos = 0;
 VoidButton* datamanager [100];

 virtual void AddButton  (VoidButton *button);
 virtual void Manager    ();
};

struct ButtonPanels: public VoidButton, ButtonManager
{
 ButtonPanels           (int x_, int y_, int sizex_, int sizey_);


 virtual void Action     ();
 virtual void Manager    ();
 virtual int  InsideTest ();

};

struct TextButton: public VoidButton
{

 char buttontext [200];
 COLORREF colorbutton;

 TextButton              (int x_ ,int y_, int sizex_, int sizey_, char buttontext_ [], COLORREF colorbutton_);

 virtual void Action     ();
 virtual void DrawButton ();
 virtual int  InsideTest ();

};

struct Vertex;

struct Edge: public VoidButton
{
 COLORREF coloredge;

 Vertex* joinedvertex [2];

 Edge (COLORREF coloredge_, Vertex* joinedvertex1_, Vertex* joinedvertex2_);

 virtual void DrawButton ();
 virtual int  InsideTest ();

};

struct Vertex: public VoidButton
{

 COLORREF colorvertex;

 std::vector <Edge*> vertexedge;
 int edgenumber = 0;
 int vertexlevel = 0;


 Vertex                  (int x_ ,int y_, int sizex_, int sizey_, COLORREF colorvertex_);

 virtual void DrawButton ();
 virtual int  InsideTest ();

};


struct ImageButton: public VoidButton
{

 HDC picture;
 HDC pictureon;

 ImageButton              (int x_, int y_, int sizex_, int sizey_, HDC picture_, HDC pictureon_);

 virtual void Action      ();
 virtual void DrawButton  ();
 virtual int  InsideTest  ();

 ~ImageButton             ()
 {
  txDeleteDC              (picture);
  txDeleteDC              (pictureon);
 }

};



struct WorkWindow: public ButtonPanels
{


 std::vector <Vertex*> vertexdata;
 std::vector <Edge*>   edgedata;

 int vertexnumber = 0;
 int edgenumber   = 0;

 Vertex* workvertex;
 int workedge = -1;

 int vertexsize;

 COLORREF coloredge;
 COLORREF colorvertex;

 void    Grab                   ();
 void    AddVertex              ();
 void    ConectVertex           ();
 void    DeleteObjects          ();
 void    UpdateWorkWindow       ();
 int     RepeatEdge             (Vertex* vertex1, Vertex* vertex2);
 Vertex* AnotherVertex          (Edge* edge, Vertex* vertex);
 void    DeleteEdge             (Edge* deletededge);
 void    DeleteVertex           (Vertex* deletedvertex);
 void    DFS                    ();
 void    WorkDFS                (Vertex* vertex);
 void    BFS                    ();

 void (WorkWindow::*instrument) ();


 WorkWindow (int x_, int y_, int sizex_, int sizey_, int vertexsize_, COLORREF coloredge_,
             COLORREF colorvertex_, void (WorkWindow::*instrument_) ());


 virtual void Action            ();
 virtual void DrawButton        ();
 virtual void Manager           ();

 ~WorkWindow ();


};

struct ChangeInstrument: public ImageButton
{
 WorkWindow *holst;

 void (WorkWindow::*workinstrument) ();

 ChangeInstrument                  (int x_, int y_, int sizex_, int sizey_, HDC picture_, HDC pictureon_,
                                    WorkWindow *holst_, void (WorkWindow::*workinstrument_) ());

 virtual void Action               ();

};



int main ()
{

 txCreateWindow (685, 785);

 ButtonManager windowsmanager = {};


 WorkWindow   workwindow       (35, 35, 600, 600, 30, TX_RED, TX_RED, &WorkWindow::AddVertex);
 ButtonPanels graphinstruments (50, 675, 480, 80);

 ChangeInstrument addvertex     (50,  675, 80, 80, txLoadImage ("vertex+.bmp"),    txLoadImage ("vertex+on.bmp"),    &workwindow, &WorkWindow::AddVertex);
 ChangeInstrument grab          (130, 675, 80, 80, txLoadImage ("grab.bmp"),  txLoadImage ("grabon.bmp"),  &workwindow, &WorkWindow::Grab);
 ChangeInstrument conectvertex  (210, 675, 80, 80, txLoadImage ("conect.bmp"), txLoadImage ("conecton.bmp"), &workwindow, &WorkWindow::ConectVertex);
 ChangeInstrument deleteobjects (290, 675, 80, 80, txLoadImage ("delete.bmp"), txLoadImage ("deleteon.bmp"), &workwindow, &WorkWindow::DeleteObjects);
 ChangeInstrument DFS_          (370, 675, 80, 80, txLoadImage ("DFS.bmp"),    txLoadImage ("DFSon.bmp"),    &workwindow, &WorkWindow::DFS);
 ChangeInstrument BFS_          (450, 675, 80, 80, txLoadImage ("BFS.bmp"),  txLoadImage ("BFSon.bmp"),  &workwindow, &WorkWindow::BFS);


 windowsmanager.AddButton      (&workwindow);
 windowsmanager.AddButton      (&graphinstruments);

 graphinstruments.AddButton    (&addvertex);
 graphinstruments.AddButton    (&grab);
 graphinstruments.AddButton    (&conectvertex);
 graphinstruments.AddButton    (&deleteobjects);
 graphinstruments.AddButton    (&DFS_);
 graphinstruments.AddButton    (&BFS_);




 windowsmanager.Manager        ();

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ChangeInstrument::ChangeInstrument     (int x_, int y_, int sizex_, int sizey_, HDC picture_, HDC pictureon_,
                                        WorkWindow *holst_, void (WorkWindow::*workinstrument_) ()):
                  ImageButton          (x_, y_, sizex_, sizey_, picture_, pictureon_),
                  holst                (holst_),
                  workinstrument       (workinstrument_)
                 {
                  DrawButton ();
                 }

void ChangeInstrument::Action ()
                      {
                       holst -> instrument = workinstrument;

                       txBitBlt (txDC (), x, y, sizex, sizey, pictureon, 0, 0);
                       txSleep  (64);
                       txBitBlt (txDC (), x, y, sizex, sizey, picture, 0, 0);
                      }




//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


WorkWindow::WorkWindow   (int x_, int y_, int sizex_, int sizey_, int vertexsize_, COLORREF coloredge_,
                          COLORREF colorvertex_, void (WorkWindow::*instrument_) ()):
            ButtonPanels (x_, y_, sizex_, sizey_),
            vertexsize   (vertexsize_),
            coloredge    (coloredge_),
            colorvertex  (colorvertex_),
            instrument   (instrument_)
                            {
                             DrawButton ();
                            }

void WorkWindow::DrawButton ()
                {
                 txSetFillColor  (TX_WHITE);
                 txSetColor      (TX_WHITE);
                 txRectangle (x - vertexsize/2, y - vertexsize/2, x + sizex + vertexsize/2, y + sizey + vertexsize/2);

                }

void WorkWindow::Action ()
                {
                 if (this->instrument == &WorkWindow::AddVertex)
                    {
                     this -> mouseposx = txMouseX ();
                     this -> mouseposy = txMouseY ();
                     AddVertex ();
                     txSleep (200);
                    }
                 else
                    {
                     Manager ();
                    }
                }

void WorkWindow::Manager ()
                {
                 for (int i = 0; i < vertexnumber; i++)
                     {
                      vertexdata[i] -> mouseposx = txMouseX ();
                      vertexdata[i] -> mouseposy = txMouseY ();
                      if((vertexdata[i] -> InsideTest () == 1)&&
                         txMouseButtons() == 1)
                        {

                         workvertex = vertexdata [i];
                         (this->*instrument) ();
                         break;
                        }
                     }

                 }

void WorkWindow::Grab ()
                {
                 while(txMouseButtons() == 1 && InsideTest ())
                      {
                       workvertex -> x = txMouseX ();
                       workvertex -> y = txMouseY ();
                       UpdateWorkWindow ();
                      }
                }

void WorkWindow::AddVertex ()
                {
                 Vertex* newvertexpointer = new Vertex (mouseposx, mouseposy, vertexsize, vertexsize, TX_RED);

                 vertexdata.push_back (newvertexpointer);
                 vertexnumber += 1;
                }

void WorkWindow::ConectVertex ()
                {
                 while (txMouseButtons () != 2)
                       {
                        if (InsideTest ())
                           {
                            txSetColor     (coloredge);
                            txSetFillColor (coloredge);
                            txLine (workvertex -> x, workvertex -> y, mouseposx, mouseposy);

                            for(int i = 0; i < vertexnumber; i++)
                               {

                                vertexdata[i] -> mouseposx = mouseposx;
                                vertexdata[i] -> mouseposy = mouseposy;


                                if ((vertexdata[i] -> InsideTest () == 1) && (txMouseButtons () == 1))
                                   {
                                    if (workvertex == vertexdata [i])
                                       {
                                        txMessageBox ("Петли запрещены");
                                        break;

                                       }

                                    if (RepeatEdge (workvertex, vertexdata [i]) == 1)
                                       {
                                        txMessageBox ("Ребро уже есть");
                                        break;
                                       }


                                    Edge* newedgepointer = new Edge (coloredge, workvertex, vertexdata [i]);

                                    edgedata.push_back (newedgepointer);
                                    edgenumber += 1;


                                    workvertex -> vertexedge.push_back (newedgepointer);
                                    workvertex -> edgenumber += 1;



                                    vertexdata [i] -> vertexedge.push_back (newedgepointer);
                                    vertexdata [i] -> edgenumber += 1;

                                    break;
                                   }

                                UpdateWorkWindow ();
                               }

                              }

                       }
                }

void WorkWindow::DeleteEdge (Edge* deletededge)
                {
                 for (int i = 0; i < edgenumber; i++)
                     {
                      if (edgedata [i] == deletededge)
                         {
                          edgedata.erase (edgedata.begin() + i);
                          edgenumber --;
                          break;
                         }
                     }

                 for (int i = 0; i < deletededge -> joinedvertex [0] -> edgenumber; i++)
                     {
                      if (deletededge -> joinedvertex [0] -> vertexedge [i] == deletededge)
                         {
                          deletededge -> joinedvertex [0] -> vertexedge.erase (deletededge -> joinedvertex [0] -> vertexedge.begin() + i);
                          deletededge -> joinedvertex [0] -> edgenumber --;
                          break;
                         }
                     }

                 for (int i = 0; i < deletededge -> joinedvertex [1] -> edgenumber; i++)
                     {
                      if (deletededge -> joinedvertex [1] -> vertexedge [i] == deletededge)
                         {
                          deletededge -> joinedvertex [1] -> vertexedge.erase (deletededge -> joinedvertex [1] -> vertexedge.begin() + i);
                          deletededge -> joinedvertex [1] -> edgenumber --;
                          break;
                         }
                     }

                 delete deletededge;
                }

void WorkWindow::DeleteVertex (Vertex* deletedvertex)
                {
                 for (int i = 0; i < vertexnumber; i++)
                     {
                      if (vertexdata [i] == deletedvertex)
                         {
                          vertexdata.erase (vertexdata.begin() + i);
                          vertexnumber --;
                          break;
                         }
                     }

                 while (deletedvertex -> edgenumber > 0)
                       {
                        DeleteEdge (deletedvertex -> vertexedge [0]);
                       }

                 delete deletedvertex;
                }

void WorkWindow::DeleteObjects ()
                {
                 DeleteVertex (workvertex);
                 UpdateWorkWindow ();

                }

void WorkWindow::UpdateWorkWindow ()
                {
                 txEnd ();



                 DrawButton ();

                 for (int i = 0;i < this -> vertexnumber; i++)
                     {
                      vertexdata [i] -> DrawButton ();
                     }

                 for (int i = 0;i < this -> edgenumber; i++)
                     {
                      edgedata [i] -> DrawButton ();
                     }

                 txBegin ();
                }

int  WorkWindow::RepeatEdge (Vertex* vertex1, Vertex* vertex2)
                {

                 for (int i = 0;i < vertex1 -> edgenumber; i++)
                     {
                      if(vertex1 -> vertexedge [i] -> joinedvertex [0] == vertex2 ||
                         vertex1 -> vertexedge [i] -> joinedvertex [1] == vertex2)
                        {
                         return 1;
                        }
                     }

                 return 0;
                }

Vertex* WorkWindow::AnotherVertex (Edge* edge, Vertex* vertex)
                   {
                    if (vertex == edge -> joinedvertex [0])
                       {
                        return edge -> joinedvertex [1];
                       }

                    if (vertex == edge -> joinedvertex [1])
                       {
                        return edge -> joinedvertex [0];
                       }

                    return NULL;
                   }

void WorkWindow::DFS ()
                {
                 WorkDFS (workvertex);

                 for (int i = 0; i < vertexnumber; i++)
                     {
                      vertexdata [i] -> colorvertex = TX_RED;
                     }

                 for (int i = 0; i < edgenumber; i++)
                     {
                      edgedata [i] -> coloredge = TX_RED;
                     }

                 UpdateWorkWindow ();
                }

void WorkWindow::WorkDFS (Vertex* vertex)
                {
                 vertex -> colorvertex = TX_YELLOW;
                 UpdateWorkWindow ();

                 txSleep (1000);

                 for (int i = 0; i < vertex -> edgenumber; i++)
                     {
                      vertex -> vertexedge [i] -> coloredge = TX_YELLOW;

                      if (txExtractColor (AnotherVertex (vertex -> vertexedge [i], vertex) -> colorvertex, TX_RED) == 128)
                         {
                          UpdateWorkWindow ();

                          txSleep (500);

                          WorkDFS (AnotherVertex (vertex -> vertexedge [i], vertex));
                         }
                     }


                 vertex -> colorvertex = TX_GREEN;
                 for (int i = 0; i < vertex -> edgenumber; i++)
                     {
                      vertex -> vertexedge [i] -> coloredge = TX_GREEN;
                     }
                 UpdateWorkWindow ();
                 txSleep (1000);
                }

void WorkWindow::BFS ()
                {
                 std::queue <Vertex*> vertexq;
                 vertexq.push (workvertex);
                 workvertex -> colorvertex = TX_YELLOW;
                 workvertex -> vertexlevel = 1;
                 UpdateWorkWindow ();
                 txSleep (1000);

                 while (vertexq.size () > 0)
                       {
                        for (int i = 0; i < vertexq.front () -> edgenumber; i++)
                            {
                             vertexq.front () -> vertexedge [i] -> coloredge = TX_YELLOW;
                             if (txExtractColor (AnotherVertex (vertexq.front () -> vertexedge [i], vertexq.front ()) -> colorvertex, TX_RED) == 128)
                                {
                                 AnotherVertex (vertexq.front () -> vertexedge [i], vertexq.front ()) -> colorvertex = TX_YELLOW;
                                 AnotherVertex (vertexq.front () -> vertexedge [i], vertexq.front ()) -> vertexlevel = vertexq.front () -> vertexlevel + 1;
                                 vertexq.push  (AnotherVertex (vertexq.front () -> vertexedge [i], vertexq.front ()));
                                }

                            }

                        vertexq.front () -> colorvertex = TX_GREEN;

                        for (int i = 0; i < vertexq.front () -> edgenumber; i++)
                            {
                             vertexq.front () -> vertexedge [i] -> coloredge = TX_GREEN;
                            }


                        vertexq.pop ();
                        UpdateWorkWindow ();
                        txSleep (1000);
                       }

                 for (int i = 0; i < vertexnumber; i++)
                     {

                      vertexdata [i] -> colorvertex = TX_RED;
                      vertexdata [i] -> vertexlevel = 0;
                     }

                 for (int i = 0; i < edgenumber; i++)
                     {
                      edgedata [i] -> coloredge = TX_RED;
                     }

                 UpdateWorkWindow ();
                }

WorkWindow::~WorkWindow ()
           {
            while (vertexnumber > 0)
                  {
                   DeleteVertex (vertexdata [0]);
                  }
           }


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TextButton::TextButton  (int x_ ,int y_, int sizex_, int sizey_, char buttontext_ [], COLORREF colorbutton_):
            VoidButton  (x_, y_, sizex_, sizey_),
            colorbutton (colorbutton_)
           {
            strcpy      (buttontext, buttontext_);
            DrawButton  ();
           }

void TextButton::Action ()
                {

                }

void TextButton::DrawButton ()
                {
                 txSetFillColor (colorbutton);
                 txSetColor     (colorbutton);
                 txRectangle    (x, y, x+sizex, y + sizey);


                 txTextOut      (x, y, buttontext);
                }

int  TextButton::InsideTest ()
                 {
                  int mouseposx = txMouseX ();
                  int mouseposy = txMouseY ();

                  if(mouseposx >= this -> x && mouseposx <= this -> x + this -> sizex &&
                     mouseposy >= this -> y && mouseposy <= this -> y + this -> sizey)
                    {
                     this -> mouseposx = mouseposx;
                     this -> mouseposy = mouseposy;

                     return 1;
                    }
                  else
                    return 0;
                 }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ImageButton::ImageButton (int x_, int y_, int sizex_, int sizey_, HDC picture_, HDC pictureon_):
             VoidButton  (x_, y_, sizex_, sizey_),
             picture     (picture_),
             pictureon   (pictureon_)
            {
             DrawButton ();
             if (picture == NULL)
                txMessageBox ("Картинка не найдена");

             if (pictureon == NULL)
                txMessageBox ("Картинка не найдена");
            }

void ImageButton::Action ()
                 {

                 }

void ImageButton::DrawButton ()
                 {
                  txBitBlt (txDC (), x, y, sizex, sizey, picture, 0, 0);
                 }

int  ImageButton::InsideTest ()
                 {
                  int mouseposx = txMouseX ();
                  int mouseposy = txMouseY ();

                  if(mouseposx >= this -> x && mouseposx <= this -> x + this -> sizex &&
                     mouseposy >= this -> y && mouseposy <= this -> y + this -> sizey)
                    {
                     this -> mouseposx = mouseposx;
                     this -> mouseposy = mouseposy;

                     return 1;
                    }
                  else
                    return 0;
                 }
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ButtonPanels::ButtonPanels (int x_, int y_, int sizex_, int sizey_):
               VoidButton  (x_, y_, sizex_, sizey_)
              {

              }

void ButtonPanels::Action ()
                  {
                   Manager ();
                  }

void ButtonPanels::Manager ()
                  {
                   for (int i = 0; i < lastpos; i++)
                       {
                        if(datamanager[i] -> InsideTest () &&
                           txMouseButtons() == 1)
                          {
                           datamanager[i] -> mouseposx = txMouseX ();
                           datamanager[i] -> mouseposy = txMouseY ();
                           datamanager[i] -> Action               ();
                           break;
                          }
                        }
                  }

int  ButtonPanels::InsideTest ()
                  {
                   int mouseposx = txMouseX ();
                   int mouseposy = txMouseY ();

                   if(mouseposx >= this -> x && mouseposx <= this -> x + this -> sizex &&
                      mouseposy >= this -> y && mouseposy <= this -> y + this -> sizey)
                     {
                      this -> mouseposx = mouseposx;
                      this -> mouseposy = mouseposy;

                      return 1;
                     }
                   else
                      return 0;
                  }



//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void ButtonManager::Manager ()
                   {
                    for (int a = 0; !GetAsyncKeyState ('E'); a++)
                        {
                         for (int i = 0; i < lastpos; i++)
                             {
                              if(datamanager[i] -> InsideTest () &&
                                 txMouseButtons() == 1)
                                {
                                 datamanager[i] -> mouseposx = txMouseX ();
                                 datamanager[i] -> mouseposy = txMouseY ();
                                 datamanager[i] -> Action               ();
                                }
                             }
                        }
                   }

void ButtonManager::AddButton (VoidButton *button)
               {
                datamanager [lastpos] = button;
                lastpos ++;
               }


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

VoidButton::VoidButton (int x_, int y_, int sizex_, int sizey_):
            x          (x_),
            y          (y_),
            sizex      (sizex_),
            sizey      (sizey_)

           {

           }

void VoidButton::Action ()
                {
                 txMessageBox ("если вы видите эту надпись, то скорее всего ваша программа работает не правильно");
                }

void VoidButton::DrawButton ()
                {
                 txMessageBox ("если вы видите эту надпись, то скорее всего ваша программа работает неправильно");
                }

int VoidButton::InsideTest ()
               {
                txMessageBox ("если вы видите эту надпись, то скорее всего ваша программа работает не правильно");
               }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Vertex::Vertex      (int x_ ,int y_, int sizex_, int sizey_, COLORREF colorvertex_):
        VoidButton  (x_, y_, sizex_, sizey_),
        colorvertex (colorvertex_)

       {
        DrawButton  ();
       }

void Vertex::DrawButton ()
            {
             txSetColor     (colorvertex);
             txSetFillColor (colorvertex);
             txCircle (x, y, sizex/2);
             if (vertexlevel != 0)
                {
                 txSetColor (TX_BLACK);
                 char vertexlevelstring [10];
                 sprintf (vertexlevelstring, "%d", vertexlevel);
                 txTextOut (x - sizex/4, y - sizex/4, vertexlevelstring);
                }


            }

int  Vertex::InsideTest ()
            {

             if (((mouseposx - x)*(mouseposx - x) + (mouseposy - y)*(mouseposy - y)) < ((sizex/2)*(sizex/2)))
                {

                 return 1;
                }
             else
                 return 0;

            }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Edge::Edge             (COLORREF coloredge_, Vertex* joinedvertex1_, Vertex* joinedvertex2_):
      VoidButton       (0, 0, 0, 0),
      coloredge        (coloredge_)

     {
      joinedvertex [0] = joinedvertex1_;
      joinedvertex [1] = joinedvertex2_;

      DrawButton  ();
     }

void Edge::DrawButton ()
          {
           txSetColor     (coloredge);
           txSetFillColor (coloredge);
           txLine (joinedvertex [0] -> x, joinedvertex [0] -> y, joinedvertex [1] -> x, joinedvertex [1] -> y);
          }

int  Edge::InsideTest ()
          {
          }
