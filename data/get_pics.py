#!/usr/bin/env python

import sys
from os import mkdir, chdir, path
from apiclient.discovery import build
import requests

NUM = 10

def download_images(links):
    for i, l in enumerate(links):
        try:
            r = requests.get(l)
            ext = r.headers['content-type'].split('/')[1]
            f = open(str(i) + '.' + ext, 'w')
            f.write(r.content)
            f.close()
        except requests.exceptions.ConnectionError:
            print 'error accessing ' + l

if __name__ == '__main__':

    query = ' '.join(sys.argv[1:])
    directory = '_'.join(sys.argv[1:])

    print 'query: "' + query + '"'
    print 'getting links from Google...'

    service = build("customsearch", "v1",
            developerKey="AIzaSyDx23eWflm8I0An6JZh8wN25_nDSNlobKs")

    links = []
    for page in xrange(1,11):

        # To tweak this, see the query params here:
        # https://developers.google.com/custom-search/v1/using_rest#api_params
        res = service.cse().list(
          q=query,
          cx='001587704479565416834:nj-ztt4jmtm',
          searchType='image',
          imgColorType='color',
          imgType='photo',
          start=(page-1)*NUM+1
        ).execute()

        links += [i['link'] for i in res['items']]

    if not path.exists(directory):
        mkdir(directory)
    chdir(directory)

    print 'downloading images...'
    download_images(links)
