#!/usr/bin/env python

###############################################################################
# get_pics.py
#
# This program fetches images that match a query string provided on the
# command line. Each time this program is run, it makes 10 calls to Google's
# Custom Search API. The current limit is 100 calls per developer per day,
# so only run this program 10 times per day max.
#
# Created by Peter Sugihara
###############################################################################

import sys
from os import mkdir, chdir, path
from apiclient.discovery import build
import requests

PER_PAGE = 10

# Download images to the current directory given an array of URL's.
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

    # Join all arguments to form the query string.
    query = ' '.join(sys.argv[1:])

    # Name the directory after the query string.
    directory = '_'.join(sys.argv[1:])

    print 'query: "' + query + '"'
    print 'getting links from Google...'

    service = build("customsearch", "v1",
            developerKey="AIzaSyDx23eWflm8I0An6JZh8wN25_nDSNlobKs")

    # Fetch image links from Google a page at a time.
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
          start=(page-1)*PER_PAGE+1
        ).execute()

        links += [i['link'] for i in res['items']]

    # Place the image directory at the current path.
    if not path.exists(directory):
        mkdir(directory)
    chdir(directory)

    print 'downloading images...'
    download_images(links)
