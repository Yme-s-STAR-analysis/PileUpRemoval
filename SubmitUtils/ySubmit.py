r'''
    ySubmit
    `version`: vanilla
    `author`: yghuang
'''

import os
import sys
import numpy as np
from conf import Args
from yLog import yLog

logging = yLog(Args.ySubmitLogName)
logging.log(f'ySubmit logging info. can be found in {Args.ySubmitLogName}')

if len(sys.argv) != 2:
    raise Exception('Usage: python3 ySubmit.py $MODE.')
else:
    mode = sys.argv[1]
    if mode in ['run', 'normal', 'merge']:
        logging.log(f'{mode} mode set.')
    else:
        raise Exception(f'Mode must be \'run\', \'normal\' or \'merge\', and you are using {mode}')
    fileList = Args.fileList
    files = [item.rstrip('\n') for item in open(fileList, 'r').readlines()]
    logging.log(f'Reading file list from: {fileList}')
    nFiles = len(files)
    logging.log(f'{nFiles} found.')
    jobPath = Args.jobPath
    logging.log(f'Single jobs path set to be: {jobPath}')
    if not os.path.exists(jobPath):
        raise Exception(f'{jobPath} does not exist!')
    if mode == 'run':
        runNumberList = Args.runNumberList
        logging.log(f'Loading run number list from: {runNumberList}')
        runNumberLoc = Args.runNumberLoc
        logging.log(f'Loading run number location in file list: {runNumberLoc}')
        runNumers = [item.rstrip('\n') for item in open(runNumberList, 'r').readlines()]
        nRuns = len(runNumers)
        logging.log(f'{nRuns} runs found.')
        filesOfRuns = dict()
        nFilesOfRuns = [] # will be as long as runNumbers
        # make dir. and init. filesOfRuns
        for item in runNumers:
            filesOfRuns[item] = []
            os.mkdir(f'{jobPath}/{item}')
        # despatch files into each run of filesOfRuns
        fullRun = [item.split('/')[runNumberLoc] for item in files]
        for idx, item in enumerate(fullRun):
            if item in runNumers: # only consider those selected run numbers
                filesOfRuns[item].append(files[idx])
        # submit jobs
        for item in runNumers:
            nFilesOfRuns.append(len(filesOfRuns[item]))
            with open(f'{jobPath}/{item}/file.list', 'w') as f:
                for fileName in filesOfRuns[item]:
                    f.write(f'{fileName}\n')
            for lnsItem in Args.lnsList:
                os.system(f'ln -s {os.getcwd()}/{lnsItem} {jobPath}/{item}/{lnsItem}')
            for cprItem in Args.cprList:
                os.system(f'cp -r {os.getcwd()}/{cprItem} {jobPath}/{item}/{cprItem}')
            for cpItem in Args.cpList:
                os.system(f'cp {os.getcwd()}/{cpItem} {jobPath}/{item}/{cpItem}')
            os.system(f'cp {os.getcwd()}/subq.con {jobPath}/{item}/subq.con')
            logging.log(f'Run {item} has {len(filesOfRuns[item])} files, submitted.')
            os.system(f'cd {jobPath}/{item} && condor_submit subq.con')
        # show summary
        minRun = np.argmin(nFilesOfRuns)
        maxRun = np.argmax(nFilesOfRuns)
        logging.log(f'All jobs submitted, run {runNumers[minRun]} has minimum files ({nFilesOfRuns[minRun]}) and run {runNumers[maxRun]} has maximum files ({nFilesOfRuns[maxRun]})')
    if mode == 'normal':
        nFilesPerJob = Args.nFilesPerJob
        nJobs = nFiles // nFilesPerJob
        bouns = nFiles - nJobs * nFilesPerJob
        if bouns == 0:
            logging.log(f'{nFiles} files will be split into {nJobs} jobs, with {nFilesPerJob} files despatched into each job.')
        else:
            logging.log(f'{nFiles} files will be split into {nJobs+1} jobs, with {nFilesPerJob} files despatched into {nJobs} jobs, and the residue {bouns} files dealed by the last job.')
        for idx in range(nJobs):
            os.mkdir(f'{jobPath}/job{idx}')
            with open(f'{jobPath}/job{idx}/file.list', 'w') as f:
                for iFile in range(idx*nFilesPerJob, (idx+1) * nFilesPerJob):
                    f.write(f'{files[iFile]}\n')
            for lnsItem in Args.lnsList:
                os.system(f'ln -s {os.getcwd()}/{lnsItem} {jobPath}/job{idx}/{lnsItem}')
            for cprItem in Args.cprList:
                os.system(f'cp -r {os.getcwd()}/{cprItem} {jobPath}/job{idx}/{cprItem}')
            for cpItem in Args.cpList:
                os.system(f'cp {os.getcwd()}/{cpItem} {jobPath}/job{idx}/{cpItem}')
            os.system(f'cp {os.getcwd()}/subq.con {jobPath}/job{idx}/subq.con')
            os.system(f'cd {jobPath}/job{idx} && condor_submit subq.con')
            logging.log(f'Job {idx} submitted.')
        if bouns > 0:
            os.mkdir(f'{jobPath}/job{nJobs}')
            with open(f'{jobPath}/job{nJobs}/file.list', 'w') as f:
                for iFile in range(nJobs*nFilesPerJob, nFiles):
                    f.write(f'{files[iFile]}\n')
            for lnsItem in Args.lnsList:
                os.system(f'ln -s {os.getcwd()}/{lnsItem} {jobPath}/job{nJobs}/{lnsItem}')
            for cprItem in Args.cprList:
                os.system(f'cp -r {os.getcwd()}/{cprItem} {jobPath}/job{nJobs}/{cprItem}')
            for cpItem in Args.cpList:
                os.system(f'cp {os.getcwd()}/{cpItem} {jobPath}/job{nJobs}/{cpItem}')
            os.system(f'cp {os.getcwd()}/subq.con {jobPath}/job{nJobs}/subq.con')
            os.system(f'cd {jobPath}/job{nJobs} && condor_submit subq.con')
            logging.log(f'Job {idx} submitted.')
        logging.log('All jobs submitted.')
    if mode == 'merge':
        logging.log('Merge mode not finished yet.')
            