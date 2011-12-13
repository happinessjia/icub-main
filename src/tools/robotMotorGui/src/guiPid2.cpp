// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* 
 * Copyright (C) 2010 RobotCub Consortium, European Commission FP6 Project IST-004370
 * Author: Francesco Nori
 * email:  francesco.nori@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

#include "include/guiPid2.h"
#include "include/partMover.h"
#include <stdlib.h>

/*
//OLD DEFINITIONS, NOW DEPRECATED
#define CAN_SET_DEBUG_PARAM_1		46
#define CAN_GET_DEBUG_PARAM_1		47
#define CAN_SET_DEBUG_PARAM_2		48
#define CAN_GET_DEBUG_PARAM_2		49
#define CAN_SET_DEBUG_PARAM_3		14
#define CAN_GET_DEBUG_PARAM_3		15
#define CAN_SET_DEBUG_PARAM_4		16
#define CAN_GET_DEBUG_PARAM_4		17
*/

//const int debug_base = 10;
const int debug_base = 0;

//*********************************************************************************
// This callback exits from the Pid dialog
void guiPid2::destroy_win (GtkButton *dummy1, GtkWidget *dummy2)
{
  gtk_widget_destroy (trq_winPid);
  trq_winPid=NULL;
  gtk_main_quit ();
}

//*********************************************************************************
// This callback is called when the notebook change pages
void guiPid2::change_page (GtkNotebook *notebook, GtkWidget   *page,  guint page_num,  gpointer user_data)
{
	//color definitions
	GdkColor color_green;
	GdkColor color_red;
	GdkColor color_pink;
	color_pink.red=219*255;   //DB
	color_pink.green=166*255; //A6
	color_pink.blue=171*255;  //AB
	color_red.red=255*255;     //FF
	color_red.green=100*255;    //64
	color_red.blue=100*255;
	color_green.red=149*255;   //95
	color_green.green=221*255; //DD
	color_green.blue=186*255;  //BA

	gtk_label_set_markup (GTK_LABEL (note_lbl1), g_markup_printf_escaped ("Position PID"));
    gtk_label_set_markup (GTK_LABEL (note_lbl2), g_markup_printf_escaped ("Torque PID"));
    gtk_label_set_markup (GTK_LABEL (note_lbl3), g_markup_printf_escaped ("Stiffness params"));

	if (page_num==0) gtk_label_set_markup (GTK_LABEL (note_lbl1), g_markup_printf_escaped ("<span bgcolor=\"#95DDBA\">Position PID</span>"));
    if (page_num==1) gtk_label_set_markup (GTK_LABEL (note_lbl2), g_markup_printf_escaped ("<span bgcolor=\"#FF6464\">Torque PID</span>"));
    if (page_num==2) gtk_label_set_markup (GTK_LABEL (note_lbl3), g_markup_printf_escaped ("<span bgcolor=\"#DBA6AB\">Stiffness params</span>"));

/*
	if (iPid)
		iPid->getPid(*joint, &myPosPid);
	if (iTrq)
		iTrq->getTorquePid(*joint, &myTrqPid);
	if (iImp)
		iImp->getImpedance(*joint, &stiff_val, &damp_val);
*/
}


//*********************************************************************************
void guiPid2::send_pos_pid (GtkButton *button, Pid *pid)
{
  char buffer[40];

  //fprintf(stderr, "%s \n", gtk_entry_get_text((GtkEntry*) kpDes));
  pid->kp = atoi(gtk_entry_get_text((GtkEntry*) pos_kpDes));
  //fprintf(stderr, "kp changed to: %d \n", (int) pid->kp);
  pid->kd = atoi(gtk_entry_get_text((GtkEntry*) pos_kdDes));
  pid->ki = atoi(gtk_entry_get_text((GtkEntry*) pos_kiDes));
  pid->scale = atoi(gtk_entry_get_text((GtkEntry*) pos_scaleDes));
  pid->offset = atoi(gtk_entry_get_text((GtkEntry*) pos_offsetDes));
  pid->max_output = atoi(gtk_entry_get_text((GtkEntry*) pos_PWM_limitDes));
  pid->max_int = atoi(gtk_entry_get_text((GtkEntry*) pos_INT_limitDes));

  //fprintf(stderr, "Trying to send pid...");
  iPid->setPid(*joint, *pid);
  //fprintf(stderr, "...got it! ");
  iPid->getPid(*joint, pid);

  sprintf(buffer, "%d", (int) pid->kp);
  gtk_entry_set_text((GtkEntry*) pos_kpEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->kd);
  gtk_entry_set_text((GtkEntry*) pos_kdEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->ki);
  gtk_entry_set_text((GtkEntry*) pos_kiEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->scale);
  gtk_entry_set_text((GtkEntry*) pos_scaleEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->offset);
  gtk_entry_set_text((GtkEntry*) pos_offsetEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->max_output);
  gtk_entry_set_text((GtkEntry*) pos_PWM_limitEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->max_int);
  gtk_entry_set_text((GtkEntry*) pos_INT_limitEntry,  buffer);

}

//*********************************************************************************
void guiPid2::send_trq_pid (GtkButton *button, Pid *pid)
{
  char buffer[40];

  //fprintf(stderr, "%s \n", gtk_entry_get_text((GtkEntry*) kpDes));
  pid->kp = atoi(gtk_entry_get_text((GtkEntry*) trq_kpDes));
  //fprintf(stderr, "kp changed to: %d \n", (int) pid->kp);
  pid->kd = atoi(gtk_entry_get_text((GtkEntry*) trq_kdDes));
  pid->ki = atoi(gtk_entry_get_text((GtkEntry*) trq_kiDes));
  pid->scale = atoi(gtk_entry_get_text((GtkEntry*) trq_scaleDes));
  pid->offset = atoi(gtk_entry_get_text((GtkEntry*) trq_offsetDes));
  pid->max_output = atoi(gtk_entry_get_text((GtkEntry*) trq_PWM_limitDes));
  pid->max_int = atoi(gtk_entry_get_text((GtkEntry*) trq_INT_limitDes));

  //fprintf(stderr, "Trying to send pid...");
  iTrq->setTorquePid(*joint, *pid);
  //fprintf(stderr, "...got it! ");
  iTrq->getTorquePid(*joint, pid);

  sprintf(buffer, "%d", (int) pid->kp);
  gtk_entry_set_text((GtkEntry*) trq_kpEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->kd);
  gtk_entry_set_text((GtkEntry*) trq_kdEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->ki);
  gtk_entry_set_text((GtkEntry*) trq_kiEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->scale);
  gtk_entry_set_text((GtkEntry*) trq_scaleEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->offset);
  gtk_entry_set_text((GtkEntry*) trq_offsetEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->max_output);
  gtk_entry_set_text((GtkEntry*) trq_PWM_limitEntry,  buffer);
  sprintf(buffer, "%d", (int) pid->max_int);
  gtk_entry_set_text((GtkEntry*) trq_INT_limitEntry,  buffer);
}

//*********************************************************************************
void guiPid2::send_imp_pid (GtkButton *button, Pid *pid)
{
  char buffer[40];

  double stiff_val=atof(gtk_entry_get_text((GtkEntry*) imp_stiffDes));
  double damp_val=atof(gtk_entry_get_text((GtkEntry*) imp_dampDes));
  double offset_val=atof(gtk_entry_get_text((GtkEntry*) imp_offDes));
  iImp->setImpedance(*joint,  stiff_val,  damp_val);
  //iImp->setImpedanceOffset(*joint, &offset_val); //DANGEROUS,DO NOT USE! WRONG USER VALUES MAY BREAK THE ROBOT!
  iImp->getImpedance(*joint, &stiff_val, &damp_val);
  iImp->getImpedanceOffset(*joint, &offset_val);

  //update the impedance params
  sprintf(buffer, "%3.3f", stiff_val);
  gtk_entry_set_text((GtkEntry*) imp_stiffEntry,  buffer);
  sprintf(buffer, "%3.3f", damp_val);
  gtk_entry_set_text((GtkEntry*) imp_dampEntry,  buffer);
  sprintf(buffer, "%3.3f", offset_val);
  gtk_entry_set_text((GtkEntry*) imp_offEntry,  buffer);

  //update the impedance limits
  double stiff_max=0.0;
  double stiff_min=0.0;
  double damp_max=0.0;
  double damp_min=0.0;
  double off_max=0.0;
  double off_min=0.0;
  iImp->getCurrentImpedanceLimit(*joint, &stiff_min, &stiff_max, &damp_min, &damp_max);
  iTrq->getTorqueRange(*joint, &off_min, &off_max);
  sprintf(buffer, "%3.3f", stiff_min);
  gtk_entry_set_text((GtkEntry*) imp_stiffMin,  buffer);
  sprintf(buffer, "%3.3f", stiff_max);
  gtk_entry_set_text((GtkEntry*) imp_stiffMax,  buffer);
  sprintf(buffer, "%3.3f", damp_min);
  gtk_entry_set_text((GtkEntry*) imp_dampMin,  buffer);
  sprintf(buffer, "%3.3f", damp_max);
  gtk_entry_set_text((GtkEntry*) imp_dampMax,  buffer);

  sprintf(buffer, "%3.3f", off_min);
  gtk_entry_set_text((GtkEntry*) imp_offMin,  buffer);
  sprintf(buffer, "%3.3f", off_max);
  gtk_entry_set_text((GtkEntry*) imp_offMax,  buffer);
}

//*********************************************************************************
void guiPid2::send_dbg_pid (GtkButton *button, Pid *pid)
{
  if (iDbg==0) 
  {
	  fprintf(stderr, "WARN: Debug interface not enabled.\n");
	  return;
  }
  char buffer[40];

  double debug_param [8];
  for (int i=0; i<8; i++) debug_param[i] =0;
  debug_param[0]=atoi(gtk_entry_get_text((GtkEntry*) dbg_debug0Des));
  debug_param[1]=atoi(gtk_entry_get_text((GtkEntry*) dbg_debug1Des));
  debug_param[2]=atoi(gtk_entry_get_text((GtkEntry*) dbg_debug2Des));
  debug_param[3]=atoi(gtk_entry_get_text((GtkEntry*) dbg_debug3Des));
  debug_param[4]=atoi(gtk_entry_get_text((GtkEntry*) dbg_debug4Des));
  debug_param[5]=atoi(gtk_entry_get_text((GtkEntry*) dbg_debug5Des));
  debug_param[6]=atoi(gtk_entry_get_text((GtkEntry*) dbg_debug6Des));
  debug_param[7]=atoi(gtk_entry_get_text((GtkEntry*) dbg_debug7Des));

  //double t;
  //iDbg->setDebugReferencePosition(0,123);
  //iDbg->getDebugReferencePosition(0,&t);
  iDbg->setDebugParameter(*joint, debug_base+0,  debug_param[0]); debug_param[0] = 0;
  iDbg->getDebugParameter(*joint, debug_base+0, &debug_param[0]);
  iDbg->setDebugParameter(*joint, debug_base+1,  debug_param[1]); debug_param[1] = 0;
  iDbg->getDebugParameter(*joint, debug_base+1, &debug_param[1]);
  iDbg->setDebugParameter(*joint, debug_base+2,  debug_param[2]); debug_param[2] = 0;
  iDbg->getDebugParameter(*joint, debug_base+2, &debug_param[2]);
  iDbg->setDebugParameter(*joint, debug_base+3,  debug_param[3]); debug_param[3] = 0;
  iDbg->getDebugParameter(*joint, debug_base+3, &debug_param[3]);
  iDbg->setDebugParameter(*joint, debug_base+4,  debug_param[4]); debug_param[4] = 0;
  iDbg->getDebugParameter(*joint, debug_base+4, &debug_param[4]);
  iDbg->setDebugParameter(*joint, debug_base+5,  debug_param[5]); debug_param[5] = 0;
  iDbg->getDebugParameter(*joint, debug_base+5, &debug_param[5]);
  iDbg->setDebugParameter(*joint, debug_base+6,  debug_param[6]); debug_param[6] = 0;
  iDbg->getDebugParameter(*joint, debug_base+6, &debug_param[6]);
  iDbg->setDebugParameter(*joint, debug_base+7,  debug_param[7]); debug_param[7] = 0;
  iDbg->getDebugParameter(*joint, debug_base+7, &debug_param[7]);

  sprintf(buffer, "%d", (int) debug_param[0]);
  gtk_entry_set_text((GtkEntry*) dbg_debug0Entry,  buffer);
  sprintf(buffer, "%d", (int) debug_param[1]);
  gtk_entry_set_text((GtkEntry*) dbg_debug1Entry,  buffer);
  sprintf(buffer, "%d", (int) debug_param[2]);
  gtk_entry_set_text((GtkEntry*) dbg_debug2Entry,  buffer);
  sprintf(buffer, "%d", (int) debug_param[3]);
  gtk_entry_set_text((GtkEntry*) dbg_debug3Entry,  buffer);
  sprintf(buffer, "%d", (int) debug_param[4]);
  gtk_entry_set_text((GtkEntry*) dbg_debug4Entry,  buffer);
  sprintf(buffer, "%d", (int) debug_param[5]);
  gtk_entry_set_text((GtkEntry*) dbg_debug5Entry,  buffer);
  sprintf(buffer, "%d", (int) debug_param[6]);
  gtk_entry_set_text((GtkEntry*) dbg_debug6Entry,  buffer);
  sprintf(buffer, "%d", (int) debug_param[7]);
  gtk_entry_set_text((GtkEntry*) dbg_debug7Entry,  buffer);
}

//*********************************************************************************
void guiPid2::displayPidValue(int k, GtkWidget *inv,GtkWidget *entry, int posX, int posY, const char *label, bool small)
{
  char buffer[40];
  GtkWidget *frame = gtk_frame_new (label);

  if (!small)
  {
	gtk_fixed_put	(GTK_FIXED(inv), frame, posX+0, posY);
	gtk_fixed_put	(GTK_FIXED(inv), entry, posX+30, posY+30);
	gtk_widget_set_size_request 	(frame, 130, 60);
	gtk_widget_set_size_request 	(entry, 60, 20);
  }
  else
  {
	gtk_fixed_put	(GTK_FIXED(inv), frame, posX+0, posY);
	gtk_fixed_put	(GTK_FIXED(inv), entry, posX+10, posY+30);
	gtk_widget_set_size_request 	(frame, 60, 60);
	gtk_widget_set_size_request 	(entry, 40, 20);
  }

  gtk_editable_set_editable ((GtkEditable*) entry, FALSE);
  sprintf(buffer, "%d", k);
  gtk_entry_set_text((GtkEntry*) entry,  buffer);
  return;
}

//*********************************************************************************
void guiPid2::displayPidValue(double k, GtkWidget *inv,GtkWidget *entry, int posX, int posY, const char *label, bool small)
{
  char buffer[40];
  GtkWidget *frame = gtk_frame_new (label);

  if (!small)
  {
	gtk_fixed_put	(GTK_FIXED(inv), frame, posX+0, posY);
	gtk_fixed_put	(GTK_FIXED(inv), entry, posX+30, posY+30);
	gtk_widget_set_size_request 	(frame, 130, 60);
	gtk_widget_set_size_request 	(entry, 60, 20);
  }
  else
  {
	gtk_fixed_put	(GTK_FIXED(inv), frame, posX+0, posY);
	gtk_fixed_put	(GTK_FIXED(inv), entry, posX+10, posY+30);
	gtk_widget_set_size_request 	(frame, 60, 60);
	gtk_widget_set_size_request 	(entry, 40, 20);
  }

  gtk_editable_set_editable ((GtkEditable*) entry, FALSE);
  sprintf(buffer, "%3.3f", k);
  gtk_entry_set_text((GtkEntry*) entry,  buffer);
  return;
}

//*********************************************************************************
void guiPid2::changePidValue(int k, GtkWidget *inv,GtkWidget *entry, int posX, int posY, const char * label, bool small)
{
  char buffer[40];
  GtkWidget *frame = gtk_frame_new (label);

  gtk_fixed_put	(GTK_FIXED(inv), frame, posX+20, posY);
  gtk_fixed_put	(GTK_FIXED(inv), entry, posX+50, posY+30);
  gtk_widget_set_size_request 	(frame, 130, 60);
  gtk_widget_set_size_request 	(entry, 60, 20);
  gtk_editable_set_editable ((GtkEditable*) entry, TRUE);
  sprintf(buffer, "%d", k);
  gtk_entry_set_text((GtkEntry*) entry,  buffer);
  return;
}

//*********************************************************************************
void guiPid2::changePidValue(double k, GtkWidget *inv,GtkWidget *entry, int posX, int posY, const char * label, bool small)
{
  char buffer[40];
  GtkWidget *frame = gtk_frame_new (label);

  gtk_fixed_put	(GTK_FIXED(inv), frame, posX+20, posY);
  gtk_fixed_put	(GTK_FIXED(inv), entry, posX+50, posY+30);
  gtk_widget_set_size_request 	(frame, 130, 60);
  gtk_widget_set_size_request 	(entry, 60, 20);
  gtk_editable_set_editable ((GtkEditable*) entry, TRUE);
  sprintf(buffer, "%3.3f", k);
  gtk_entry_set_text((GtkEntry*) entry,  buffer);
  return;
}
//*********************************************************************************
void guiPid2::guiPid2(void *button, void* data)
{
  int*        new_joint = ((gtkClassData*)(data))->indexPointer;
  partMover * new_part  = ((gtkClassData*)(data))->partPointer;

  if (joint!=0 && (*new_joint != *joint || new_part != currentPart))
  {
	  if (trq_winPid != NULL) destroy_win(NULL, NULL);
  }

  if (trq_winPid != NULL) 
  {
	  gtk_window_set_keep_above    (GTK_WINDOW(trq_winPid),true);
	  return;
  }

  gtkClassData* currentClassData = (gtkClassData*) data;
  currentPart = currentClassData->partPointer;
  joint  = currentClassData->indexPointer;
  iPid  = currentPart->get_IPidControl();
  iTrq  = currentPart->get_ITorqueControl();
  iImp  = currentPart->get_IImpedanceControl();
  iDbg  = currentPart->get_IDebugControl();
  iVel  = currentPart->get_IVelocityControl();

  //GtkWidget *winPid = NULL;

  GtkWidget *button_Pos_Send;
  GtkWidget *button_Pos_Close;
  GtkWidget *button_Trq_Send;
  GtkWidget *button_Trq_Close;
  GtkWidget *button_Imp_Send;
  GtkWidget *button_Imp_Close;
  GtkWidget *button_Dbg_Send;
  GtkWidget *button_Dbg_Close;
  Pid myPosPid(0, 0, 0, 0, 0, 0);
  Pid myTrqPid(0, 0, 0, 0, 0, 0);
  double stiff_val=0;
  double damp_val=0;
  double stiff_max=0;
  double damp_max=0;
  double off_max=0;
  double stiff_min=0;
  double damp_min=0;
  double off_min=0;
  double offset_val=0;
  double debug_param [8];
  for (int i=0; i<8; i++) debug_param[i] =0;

  iImp->getCurrentImpedanceLimit(*joint, &stiff_min, &stiff_max, &damp_min, &damp_max);
  iTrq->getTorqueRange(*joint, &off_min, &off_max);

  iPid->getPid(*joint, &myPosPid);
  iTrq->getTorquePid(*joint, &myTrqPid);
  iImp->getImpedance(*joint, &stiff_val, &damp_val);
  iImp->getImpedanceOffset(*joint, &offset_val);
  if (iDbg != 0)
  {
	iDbg->getDebugParameter(*joint, debug_base+0, &debug_param[0]);
	iDbg->getDebugParameter(*joint, debug_base+1, &debug_param[1]);
	iDbg->getDebugParameter(*joint, debug_base+2, &debug_param[2]);
	iDbg->getDebugParameter(*joint, debug_base+3, &debug_param[3]);
	iDbg->getDebugParameter(*joint, debug_base+4, &debug_param[4]);
	iDbg->getDebugParameter(*joint, debug_base+5, &debug_param[5]);
	iDbg->getDebugParameter(*joint, debug_base+6, &debug_param[6]);
	iDbg->getDebugParameter(*joint, debug_base+7, &debug_param[7]);
  }
  else
  {
	fprintf(stderr, "WARN: Debug interface not enabled.\n");
  }

#if 0
  //ImpedanceOffset test
  iImp->setImpedanceOffset(*joint, 0.2); 
  iImp->getImpedanceOffset(*joint, &offset_val);
#endif

  //adding a popup window
  trq_winPid = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  char title[255];
  sprintf(title,"Pid control %s JNT:%d",currentPart->partLabel, *joint);
  gtk_window_set_title (GTK_WINDOW (trq_winPid), title);

  //adding a notebook
  note_book = gtk_notebook_new ();
  //adding notebook labels
  note_lbl1 = gtk_label_new("Position PID");
  note_lbl2 = gtk_label_new("Torque PID");
  note_lbl3 = gtk_label_new("Stiffness params");
  note_lbl4 = gtk_label_new("Debug1");
  note_lbl5 = gtk_label_new("Debug2");

  note_pag1 = gtk_fixed_new ();
  note_pag2 = gtk_fixed_new ();
  note_pag3 = gtk_fixed_new ();
  note_pag4 = gtk_fixed_new ();
  note_pag5 = gtk_fixed_new ();

  gtk_notebook_append_page   (GTK_NOTEBOOK(note_book), note_pag1, note_lbl1);
  gtk_notebook_append_page   (GTK_NOTEBOOK(note_book), note_pag2, note_lbl2);
  gtk_notebook_append_page   (GTK_NOTEBOOK(note_book), note_pag3, note_lbl3);

  if (iDbg != 0)
  {
	  gtk_notebook_append_page   (GTK_NOTEBOOK(note_book), note_pag4, note_lbl4);
	  gtk_notebook_append_page   (GTK_NOTEBOOK(note_book), note_pag5, note_lbl5);
  }

  //adding a set of display
  //inv = gtk_fixed_new ();

  gtk_container_add (GTK_CONTAINER (trq_winPid), note_book);

  // ------ DEBUG CONTROL ------
  //debug_param0
  dbg_debug0Entry   =  gtk_entry_new();
  displayPidValue((int) debug_param[0], note_pag4, dbg_debug0Entry, 0, 0, "Current Debug0");
  //debug_param0 desired
  dbg_debug0Des   =  gtk_entry_new();
  changePidValue((int) debug_param[0], note_pag4, dbg_debug0Des, 110, 0, "Desired Debug0");
  //debug_param1
  dbg_debug1Entry   =  gtk_entry_new();
  displayPidValue((int) debug_param[1], note_pag4, dbg_debug1Entry, 0, 70, "Current Debug1");
  //debug_param1 desired
  dbg_debug1Des   =  gtk_entry_new();
  changePidValue((int) debug_param[1], note_pag4, dbg_debug1Des, 110, 70, "Desired Debug1");
  //debug_param2
  dbg_debug2Entry   =  gtk_entry_new();
  displayPidValue((int) debug_param[2], note_pag4, dbg_debug2Entry, 0, 140, "Current debug2");
  //debug_param2 desired
  dbg_debug2Des   =  gtk_entry_new();
  changePidValue((int) debug_param[2], note_pag4, dbg_debug2Des, 110, 140, "Desired debug2");
  //debug_param3
  dbg_debug3Entry   =  gtk_entry_new();
  displayPidValue((int) debug_param[3], note_pag4, dbg_debug3Entry, 0, 210, "Current debug3\nbEMF shift factor");
  //debug_param3 desired
  dbg_debug3Des   =  gtk_entry_new();
  changePidValue((int) debug_param[3], note_pag4, dbg_debug3Des, 110, 210, "Desired debug3\nbEMF shift factor");
  //debug_param4
  dbg_debug4Entry   =  gtk_entry_new();
  displayPidValue((int) debug_param[4], note_pag4, dbg_debug4Entry, 0, 280, "Current Debug4\nbEMF gain");
  //debug_param4 desired
  dbg_debug4Des   =  gtk_entry_new();
  changePidValue((int) debug_param[4], note_pag4, dbg_debug4Des, 110, 280, "Desired Debug4\nbEMF gain");
  //debug_param5
  dbg_debug5Entry   =  gtk_entry_new();
  displayPidValue((int) debug_param[5], note_pag4, dbg_debug5Entry, 0, 350, "Current Debug5");
  //debug_param5 desired
  dbg_debug5Des   =  gtk_entry_new();
  changePidValue((int) debug_param[5], note_pag4, dbg_debug5Des, 110, 350, "Desired Debug5");
  //debug_param6
  dbg_debug6Entry   =  gtk_entry_new();
  displayPidValue((int) debug_param[6], note_pag4, dbg_debug6Entry, 0, 420, "Current debug6");
  //debug_param6 desired
  dbg_debug6Des   =  gtk_entry_new();
  changePidValue((int) debug_param[6], note_pag4, dbg_debug6Des, 110, 420, "Desired debug6");
  //debug_param7
  dbg_debug7Entry   =  gtk_entry_new();
  displayPidValue((int) debug_param[7], note_pag4, dbg_debug7Entry, 0, 490, "Current debug7\nspeed damping (torque)");
  //debug_param7 desired
  dbg_debug7Des   =  gtk_entry_new();
  changePidValue((int) debug_param[7], note_pag4, dbg_debug7Des, 110, 490, "Desired debug7\nspeed damping (torque)");

  // ------ POSITION CONTROL ------
  //kp
  pos_kpEntry   =  gtk_entry_new();
  displayPidValue((int) myPosPid.kp, note_pag1, pos_kpEntry, 0, 0, "Current Position Kp");
  //kp desired
  pos_kpDes   =  gtk_entry_new();
  changePidValue((int) myPosPid.kp, note_pag1, pos_kpDes, 110, 0, "Desired Position Kp");
  //kd
  pos_kdEntry   =  gtk_entry_new();
  displayPidValue((int) myPosPid.kd, note_pag1, pos_kdEntry, 0, 70, "Current Position Kd");
  //kd desired
  pos_kdDes   =  gtk_entry_new();
  changePidValue((int) myPosPid.kd, note_pag1, pos_kdDes, 110, 70, "Desired Position Kd");
  //ki
  pos_kiEntry   =  gtk_entry_new();
  displayPidValue((int) myPosPid.ki, note_pag1, pos_kiEntry, 0, 140, "Current Position Ki");
  //ki desired
  pos_kiDes   =  gtk_entry_new();
  changePidValue((int) myPosPid.ki, note_pag1, pos_kiDes, 110, 140, "Desired Position Ki");
  //scale
  pos_scaleEntry   =  gtk_entry_new();
  displayPidValue((int) myPosPid.scale, note_pag1, pos_scaleEntry, 0, 210, "Current Position shift");
  //scale desired
  pos_scaleDes   =  gtk_entry_new();
  changePidValue((int) myPosPid.scale, note_pag1, pos_scaleDes, 110, 210, "Desired Position shift");
  //offset
  pos_offsetEntry   =  gtk_entry_new();
  displayPidValue((int) myPosPid.offset, note_pag1, pos_offsetEntry, 0, 280, "Current Position offset");
  //offset desired
  pos_offsetDes   =  gtk_entry_new();
  changePidValue((int) myPosPid.offset, note_pag1, pos_offsetDes, 110, 280, "Desired Position offset");

  //PWM limit
  pos_PWM_limitEntry   =  gtk_entry_new();
  displayPidValue((int) myPosPid.max_output, note_pag1, pos_PWM_limitEntry, 0, 360, "Current PWM limit");
  //PWM limit desired
  pos_PWM_limitDes=  gtk_entry_new();
  changePidValue((int) myPosPid.max_output, note_pag1, pos_PWM_limitDes, 110, 360, "Desired PWM limit");
  //INTEGRAL limit
  pos_INT_limitEntry   =  gtk_entry_new();
  displayPidValue((int) myPosPid.max_int, note_pag1, pos_INT_limitEntry, 0, 430, "Current Integral limit");
  //INTEGRAL limit desired
  pos_INT_limitDes=  gtk_entry_new();
  changePidValue((int) myPosPid.max_int, note_pag1, pos_INT_limitDes, 110, 430, "Desired Integral limit");

    // ------ TORQUE CONTROL ------
  //kp
  trq_kpEntry   =  gtk_entry_new();
  displayPidValue((int) myTrqPid.kp, note_pag2, trq_kpEntry, 0, 0, "Current Torque Kp");
  //kp desired
  trq_kpDes   =  gtk_entry_new();
  changePidValue((int) myTrqPid.kp, note_pag2, trq_kpDes, 110, 0, "Desired Torque Kp");
  //kd
  trq_kdEntry   =  gtk_entry_new();
  displayPidValue((int) myTrqPid.kd, note_pag2, trq_kdEntry, 0, 70, "Current Torque Kd");
  //kd desired
  trq_kdDes   =  gtk_entry_new();
  changePidValue((int) myTrqPid.kd, note_pag2, trq_kdDes, 110, 70, "Desired Torque Kd");
  //ki
  trq_kiEntry   =  gtk_entry_new();
  displayPidValue((int) myTrqPid.ki, note_pag2, trq_kiEntry, 0, 140, "Current Torque Ki");
  //ki desired
  trq_kiDes   =  gtk_entry_new();
  changePidValue((int) myTrqPid.ki, note_pag2, trq_kiDes, 110, 140, "Desired Torque Ki");
  //scale
  trq_scaleEntry   =  gtk_entry_new();
  displayPidValue((int) myTrqPid.scale, note_pag2, trq_scaleEntry, 0, 210, "Current Torque shift");
  //scale desired
  trq_scaleDes   =  gtk_entry_new();
  changePidValue((int) myTrqPid.scale, note_pag2, trq_scaleDes, 110, 210, "Desired Torque shift");
  //offset
  trq_offsetEntry   =  gtk_entry_new();
  displayPidValue((int) myTrqPid.offset, note_pag2, trq_offsetEntry, 0, 280, "Current Torque offset");
  //offset desired
  trq_offsetDes   =  gtk_entry_new();
  changePidValue((int) myTrqPid.offset, note_pag2, trq_offsetDes, 110, 280, "Desired Torque offset");

  //PWM limit
  trq_PWM_limitEntry   =  gtk_entry_new();
  displayPidValue((int) myTrqPid.max_output, note_pag2, trq_PWM_limitEntry, 0, 360, "Current PWM limit");
  //PWM limit desired
  trq_PWM_limitDes=  gtk_entry_new();
  changePidValue((int) myTrqPid.max_output, note_pag2, trq_PWM_limitDes, 110, 360, "Desired PWM limit");
  //INTEGRAL limit
  trq_INT_limitEntry   =  gtk_entry_new();
  displayPidValue((int) myTrqPid.max_int, note_pag2, trq_INT_limitEntry, 0, 430, "Current Integral limit");
  //INTEGRAL limit desired
  trq_INT_limitDes=  gtk_entry_new();
  changePidValue((int) myTrqPid.max_int, note_pag2, trq_INT_limitDes, 110, 430, "Desired Integral limit");

  // ------ IMPEDANCE CONTROL ------
  //stiffness
  imp_stiffEntry = gtk_entry_new();
  displayPidValue(stiff_val, note_pag3, imp_stiffEntry, 0, 0, "Current Joint stiffness");
  //stiffness desired
  imp_stiffDes   =  gtk_entry_new();
  changePidValue(stiff_val, note_pag3, imp_stiffDes, 180, 0, "Desired Joint stiffness");
  //stiffness limits
  imp_stiffMax   =  gtk_entry_new();
  displayPidValue(stiff_max, note_pag3, imp_stiffMax, 330, 0, "Max", true);
  imp_stiffMin   =  gtk_entry_new();
  displayPidValue(stiff_min, note_pag3, imp_stiffMin, 140, 0, "Min", true);

  //damping
  imp_dampEntry = gtk_entry_new();
  displayPidValue(damp_val, note_pag3, imp_dampEntry, 0, 70, "Current Joint damping");
  //damping desired
  imp_dampDes   =  gtk_entry_new();
  changePidValue(damp_val, note_pag3, imp_dampDes, 180, 70, "Desired Joint damping");
  //damping limits
  imp_dampMax   =  gtk_entry_new();
  displayPidValue(damp_max, note_pag3, imp_dampMax, 330, 70, "Max", true);
  imp_dampMin   =  gtk_entry_new();
  displayPidValue(damp_min, note_pag3, imp_dampMin, 140, 70, "Min", true);

  //offset
  imp_offEntry = gtk_entry_new();
  displayPidValue(offset_val, note_pag3, imp_offEntry, 0, 140, "Current Joint Force off");
  //offset desired
  imp_offDes   =  gtk_entry_new();
  changePidValue(offset_val, note_pag3, imp_offDes, 180, 140, "Desired Joint Force off");
  //offset limits
  imp_offMax   =  gtk_entry_new();
  displayPidValue(off_max, note_pag3, imp_offMax, 330, 140, "Max", true);
  imp_offMin   =  gtk_entry_new();
  displayPidValue(off_min, note_pag3, imp_offMin, 140, 140, "Min", true);

  //Send buttons
  button_Pos_Send = gtk_button_new_with_mnemonic ("Send");
  button_Trq_Send = gtk_button_new_with_mnemonic ("Send");
  button_Imp_Send = gtk_button_new_with_mnemonic ("Send");
  button_Dbg_Send = gtk_button_new_with_mnemonic ("Send");
  gtk_fixed_put	(GTK_FIXED(note_pag1), button_Pos_Send, 0, 520);
  gtk_fixed_put	(GTK_FIXED(note_pag2), button_Trq_Send, 0, 520);
  gtk_fixed_put	(GTK_FIXED(note_pag3), button_Imp_Send, 0, 520);
  gtk_fixed_put	(GTK_FIXED(note_pag4), button_Dbg_Send, 0, 590);
  g_signal_connect (button_Pos_Send, "clicked", G_CALLBACK (send_pos_pid), &myPosPid);
  g_signal_connect (button_Trq_Send, "clicked", G_CALLBACK (send_trq_pid), &myTrqPid);
  g_signal_connect (button_Imp_Send, "clicked", G_CALLBACK (send_imp_pid), NULL);
  g_signal_connect (button_Dbg_Send, "clicked", G_CALLBACK (send_dbg_pid), NULL);
  gtk_widget_set_size_request     (button_Pos_Send, 120, 25);
  gtk_widget_set_size_request     (button_Trq_Send, 120, 25);
  gtk_widget_set_size_request     (button_Imp_Send, 120, 25);
  gtk_widget_set_size_request     (button_Dbg_Send, 120, 25);

  //Close
  button_Pos_Close = gtk_button_new_with_mnemonic ("Close");
  button_Trq_Close = gtk_button_new_with_mnemonic ("Close");
  button_Imp_Close = gtk_button_new_with_mnemonic ("Close");
  button_Dbg_Close = gtk_button_new_with_mnemonic ("Close");
  gtk_fixed_put	(GTK_FIXED(note_pag1), button_Pos_Close, 120, 520);
  gtk_fixed_put	(GTK_FIXED(note_pag2), button_Trq_Close, 120, 520);
  gtk_fixed_put	(GTK_FIXED(note_pag3), button_Imp_Close, 120, 520);
  gtk_fixed_put	(GTK_FIXED(note_pag4), button_Dbg_Close, 120, 590);
  g_signal_connect (button_Pos_Close, "clicked", G_CALLBACK (destroy_win), NULL);
  g_signal_connect (button_Trq_Close, "clicked", G_CALLBACK (destroy_win), NULL);
  g_signal_connect (button_Imp_Close, "clicked", G_CALLBACK (destroy_win), NULL);
  g_signal_connect (button_Dbg_Close, "clicked", G_CALLBACK (destroy_win), NULL);
  gtk_widget_set_size_request     (button_Pos_Close, 120, 25);
  gtk_widget_set_size_request     (button_Trq_Close, 120, 25);
  gtk_widget_set_size_request     (button_Imp_Close, 120, 25);
  gtk_widget_set_size_request     (button_Dbg_Close, 120, 25);

  //connection to the destroyer
  g_signal_connect (trq_winPid, "destroy",G_CALLBACK (destroy_win), NULL);
  //connect to the page switcher
  g_signal_connect (note_book, "switch-page", G_CALLBACK (change_page), NULL);

  //make it visible
  if (!GTK_WIDGET_VISIBLE (trq_winPid))
    gtk_widget_show_all (trq_winPid);
  else
    {
      gtk_widget_destroy (trq_winPid);
      trq_winPid = NULL;
    }
	
  gtk_window_set_keep_above    (GTK_WINDOW(trq_winPid),true);
  gtk_main ();

  return;
}
