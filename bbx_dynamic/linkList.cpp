#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <windows.h>
#include <string.h>
#include <assert.h>
#include "linkList.h"

//��������ʱ��ͷβָ���ָ��data��Ϊ0�Ľڵ�.
pQueue CreatedQueue(void) {
	pQueue pq = new(Queue);
	assert(pq != NULL);
	//pListNode pn = CreateNode(0);
	//assert(pn != NULL);
	pq->size = 0;
	pq->headLink = NULL; //pn;
	pq->tailLink = NULL; //pn;
	return pq;
}

pListNode CreateNode(int ind, double value) {
	pListNode pn = new(ListNode);
	assert(pn != NULL);
	pn->ind = ind;
	pn->data = value;
	pn->next = NULL;
	return pn;
}

//ɾ���ڵ���ɾ��headLinkָ��Ľڵ㣬�ı�headLinkָ��
pListNode popQueue(pQueue queue) {
	assert(queue != NULL);
	if (queue->size == 0)
		return NULL;
	pListNode pn = queue->headLink;
	queue->headLink = pn->next;
	pn->next = NULL;
	queue->size--;
	if (queue->size == 0)
		queue->tailLink = NULL;
	return pn;
}

//���ӽڵ���ڶ�β���ı�tailLinkָ��,��ӵ�һ��Ԫ��headLink��tailLink��ָ������ڵ�
void pushQueue(pQueue queue, pListNode node) {
	assert(queue != NULL);
	assert(node != NULL);
	if (queue->size == 0) {
		queue->headLink = node;
		queue->tailLink = node;
	}
	else {
		queue->tailLink->next = node;
		queue->tailLink = node;
	}
	queue->size++;
}

void DestroyQueue(pQueue *queue) {
	assert(*queue != NULL);
	while ((*queue)->size-- != 0) { //������нڵ�
		pListNode pn = popQueue(*queue);
		DestroyListNode(&pn);
	}
	//FREE(queue->headLink);
	//FREE(queue->tailLink);
	FREE(*queue);
}

std::vector<int> FindValuesQueue(pQueue *queue, double low, double high, bool *U) {
	assert(*queue != NULL);
	std::vector<int> tmps;
	pListNode p = (*queue)->headLink;
	int count = -999999;
	while (p != NULL) { //������нڵ�
		if (U[p->ind] && p->data >= low && p->data < high) {
			tmps.push_back(p->ind);
			count = 0;
		}
		else
			count++;
		if (count > 5)
			break;

		p = p->next;

	}
	return tmps;
}

std::vector<int> DeleteValuesQueue(pQueue *queue, double low, double high) {
	assert(*queue != NULL);
	std::vector<int> tmps;
	pListNode p = (*queue)->headLink;
	while (p != NULL && p->data >= low && p->data < high) { //������нڵ�
		p = p->next;
		pListNode pn = popQueue(*queue);
		tmps.push_back(pn->ind);
		DestroyListNode(&pn);
	}
	return tmps;
}


void DeleteKeyQueue(pQueue *queue, double ind) {
	assert(*queue != NULL);
	//��һ������m�Ľڵ㣬��������m����m����ָ��
	pListNode p, q;
	while ((*queue)->headLink && (*queue)->headLink->ind == ind) { //ͷ��㲻��m
		p = (*queue)->headLink;
		(*queue)->headLink = (*queue)->headLink->next;
		(*queue)->size--;
		free(p);
		return;
	}

	//��һ����m��ͷ���L�����p��һ���ڵ���m��ɾ����һ�ڵ�
	p = (*queue)->headLink;
	while (p && p->next) {
		while (p->next && p->next->ind == ind) {
			q = p->next;
			p->next = q->next;
			(*queue)->size--;
			free(q);
			return;
		}
		//��ʱp����һ���ڵ�Ϊ�ջ��߲���m����p����һ��λ��Ϊ�ջ��߲���m
		p = p->next;
	}
}

void DestroyListNode(pListNode *node) {
	assert(*node != NULL);
	(*node)->next = NULL;
	FREE(*node);
}

int LengthOfQueue(pQueue queue) {
	assert(queue != NULL);
	assert(queue->size == 0 || queue->size > 0);
	return queue->size;
}

void ShowQueue(pQueue queue) {
	pListNode pn = queue->headLink;
	if (pn == NULL)
		return;
	printf("ShowQueue Order ");
	int length = queue->size;
	while (length-- != 0) {
		printf(" [%f]", pn->data);
		pn = pn->next;
	}
	printf("\n");
}


void insertNode(pQueue queue, int ind, double value) {
	pListNode p_new = CreateNode(ind, value);
	pListNode pb = NULL, pf = NULL;

	pf = queue->headLink;
	queue->size++;

	if (queue->headLink == NULL) { // 1.ͷ���Ϊ�գ�ֱ�Ӳ���
		queue->headLink = p_new;
		return;
	}

	while ((p_new->data >= pf->data) && (pf->next != NULL)) { // 2.�������ҵ�����λ�õ�ǰ��������㣬pb,pf
		pb = pf;
		pf = pf->next;
	}

	if (p_new->data < pf->data) {// 3.�ҵ�һ��pfָ��Ľڵ��num�������Ľڵ��num����嵽pf�ڵ��ǰ�档
		if (pf == queue->headLink) {// 3.1.�ҵĽڵ���ͷ��㣬�嵽��ǰ��
			queue->headLink = p_new;
			p_new->next = pf;
		}else {// 3.2.�ҵ������м�ڵ㣬�����м�ڵ�pf��ǰ��
			pb->next = p_new;
			p_new->next = pf;
		}
	}else // 4.û���ҵ�һ���ڵ㣬�����������Ľڵ㶼�󣬲���ĩβ
		pf->next = p_new;
}