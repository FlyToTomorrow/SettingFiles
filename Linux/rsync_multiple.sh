#!/usr/bin/sh

rsync -a $1 zhifei@magic.ece.ust.hk:/home/zhifei/$2 --delete
rsync -a $1 zhifei@young.ece.ust.hk:/home/zhifei/$2 --delete
rsync -a $1 zhifei@fantasy.ece.ust.hk:/home/zhifei/$2 --delete
rsync -a $1 zhifei@iron.ece.ust.hk:/home/zhifei/$2 --delete
rsync -a $1 zhifei@passion.ece.ust.hk:/home/zhifei/$2 --delete

