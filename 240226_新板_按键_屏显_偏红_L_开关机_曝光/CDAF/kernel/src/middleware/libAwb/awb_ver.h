#ifndef _AAA_AWB_VER_H__
#define _AAA_AWB_VER_H__

#define awbVersion 12062501

/* v10030401 wangjg++
 * creat new awb lib
 * 
 *
 *
 * v10030402 wangjg++
 * 1����Ӻ��� : void awbFrameProc(void)��
 *        ��Ҫ��Բ����㲻������ȶ��Բ�����⣬���ö�֡��������ǰdecide�ķ�ʽ��
 * 2���޸�Outdoor�ж�������
 * 3���޸�outdoor���ߣ�
 * 4���޸��ֶ���ƽ�⣺cloudy
 *
 * v10030901 wangjg++
 * ��֡decide����Ȩ�أ�
 *
 * v10031001 wangjg++
 * outdoor spot[1] wp;
 * awbFlog0;
 *
 * v10031101
 * v10031102
 * v10031103
 *	1.�׵�����������wp_thr
 *	2.�޸�rgb_thrs
 *	3.
 * v10031104
 *    1.call awbCalConvergeGain() in awbNoEnoughPoint()
 *    2.sram
 * v10031104
 *   1. spot 1 wp in outdoor
 *   2. 
 * v10091501
 *   1. remove spot queue moe_queue_t dis_rgain_c0
 *
 * v10091502
 *   1. modefy spot mode.
 *
 * v10091601
 *   1. awb Run mode.
 *   2. enable awb run 1pf
 * v10092001
 *   1. lack white point use spot fix gain
 * v10102201
 *   1. correct determinant falsely A light to sward(outdoor).
 * v10102202
 *   1. correct Auto OB err for A light.
 * v10111514
 *   �������Ӱ���ȶ�(��ǿoutdoor ״̬�����ж�
 * v10111522
 *   1.��ǿscene ����
 *   2.���Ӱ칫�һ����жϾ���
 * v10111609
 *   1.��������sward��sky�ж�����
 * v10111720
 * v10111810
 * v10111815
 *   1.�������ӱ�׼��Դ��Ȩ��
 * v10112419
 * v10113011 
 * v10120310  
 * v10120314
 * v11102001
 *  1.decrease awb darkluma checkpoint
 * v12062501
 *  1.add awb moe queue CodeTbl, to speed up power on flow;
 */
#endif
