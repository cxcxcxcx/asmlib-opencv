#!/usr/bin/python

datasets = {
    'camAll_all': '.*',
    'camABDE_all': 'i.*[abde]-.*',
    'camAB_all': 'i.*[ab]-.*',
    'camAB_Q': 'i.*q[ab]-.*',
    'camA_all': 'i.*a-.*',
    'camA_Q': 'i.*qa-.*',
    'camB_Q': 'i.*qb-.*'
}
ptsLists = dict()
import re
for iName in datasets.keys():
    ptsLists[iName] = list()
    datasets[iName] = re.compile(datasets[iName])

import os
with open("./muct-landmarks/muct76-opencv.csv", "r") as f:
    for l in f:
        fields = l.split(',')
        dataName = fields[0]
        if dataName == "name":
            continue
        n = (len(fields) - 2) / 2
        filename = "jpg/%s.jpg.pts" % dataName
        with open(filename, "w") as fPts:
            fPts.write("%d\n" % n)
            prevX = 0
            prevY = 0
            for x, y in zip(fields[2::2], fields[3::2]):
                if x == '0' and y == '0':
                    # If this point doesn't exist...
                    x = prevX
                    y = prevY
                fPts.write("%.0f %.0f\n" % (float(x), float(y)))
                prevX = x
                prevY = y

        if os.path.exists("jpg/%s.jpg" % dataName):
            for iName, iReg in datasets.items():
                if iReg.match(dataName):
                    ptsLists[iName].append(filename)

for iName, iList in ptsLists.items():
    with open(iName + ".list", "w") as fLst:
        for l in iList:
            fLst.write("%s\n" % l);

