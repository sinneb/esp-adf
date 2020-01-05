/* Play mp3 file by audio pipeline

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_mem.h"
#include "audio_common.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "raw_stream.h"
#include "filter_resample.h"
#include "board.h"
#include "ringbuf.h"
#include "sounds2.h"

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

static const char *TAG = "PLAY_MP3_FLASH";
/*
   To embed it in the app binary, the mp3 file is named
   in the component.mk COMPONENT_EMBED_TXTFILES variable.
*/
extern const uint8_t adf_music_mp3_start[] asm("_binary_adf_music_mp3_start");
extern const uint8_t adf_music_mp3_end[]   asm("_binary_adf_music_mp3_end");

int mp3_music_read_cb(audio_element_handle_t el, char *buf, int len, TickType_t wait_time, void *ctx)
{
  printf("cb\n");
    static int mp3_pos;
    int read_size = adf_music_mp3_end - adf_music_mp3_start - mp3_pos;
    if (read_size == 0) {
        return AEL_IO_DONE;
    } else if (len < read_size) {
        read_size = len;
    }
    memcpy(buf, adf_music_mp3_start + mp3_pos, read_size);
    mp3_pos += read_size;
    return read_size;
}

int raw_read_cb(audio_element_handle_t el, char *buf, int len, TickType_t wait_time, void *ctx)
{
  printf("cb2\n");
  printf("len: %i",len);
  return ESP_OK;
}

void app_main(void)
{
    char *ptr = (char *)audiobit_music;
    //char *samp = malloc(1024 * sizeof(unsigned short));
    //audio_pipeline_handle_t pipeline;
    audio_element_handle_t i2s_stream_writer, raw_reader;
    ringbuf_handle_t ringbuf;
    QueueHandle_t i2s_queue, raw_queue;
    QueueSetHandle_t queue_set;
    //QueueSetMemberHandle_t queue_set_member;
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_LOGI(TAG, "[ 1 ] Start audio codec chip");
    audio_board_handle_t board_handle = audio_board_init();
    audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_BOTH, AUDIO_HAL_CTRL_START);
    printf("[ /1 ]\n");

    ESP_LOGI(TAG, "[2.2] Create i2s stream to write data to codec chip");
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg.type = AUDIO_STREAM_WRITER;
    i2s_cfg.i2s_config.sample_rate = 11000;
    i2s_cfg.out_rb_size = 8192;
    i2s_stream_writer = i2s_stream_init(&i2s_cfg);
    printf("[ /2.2 ]\n");

    ESP_LOGI(TAG, "[2.1] Create raw reader");
    raw_stream_cfg_t raw_cfg = RAW_STREAM_CFG_DEFAULT();
    raw_cfg.type = AUDIO_STREAM_READER;
    raw_reader = raw_stream_init(&raw_cfg);

    ESP_LOGI(TAG, "[3.3] Create a ringbuffer and insert it between raw reader and i2s writer");
    ringbuf = rb_create(1024, 8);

    audio_element_set_input_ringbuf(i2s_stream_writer, ringbuf);
    audio_element_set_output_ringbuf(raw_reader, ringbuf);

    i2s_queue = audio_element_get_event_queue(i2s_stream_writer);
    raw_queue = audio_element_get_event_queue(raw_reader);

    queue_set = xQueueCreateSet(10);
    xQueueAddToSet(i2s_queue, queue_set);
    xQueueAddToSet(raw_queue, queue_set);

    ESP_LOGI(TAG, "[ 4 ] Start audio elements");
    audio_element_run(i2s_stream_writer);
    audio_element_run(raw_reader);

    audio_element_resume(i2s_stream_writer, 0, 0);
    audio_element_resume(raw_reader, 0, 0);

    // memcpy to ringbuf
    rb_write(ringbuf, ptr, 8192, 100);

    int offset=8;
    while (1) {
      // sounds2.h contains an 11khz, 16 bit wavefile
      // samples are interleaved and in signed short format
      // read per byte, so two reads for left channel followed by two
      // reads for the right channel
      if(rb_bytes_available(ringbuf)>1023 && offset<54) {

        printf("ringbuf: %i\n",rb_bytes_available(ringbuf));

        //memcpy (samples, ptr+1024*sizeof(unsigned short)*offset, sizeof(samples));
        // memcpy( samp, ptr+1024*sizeof(unsigned short)*offset, 1024 );
        // // //
        // // //
        // for (i=0; i<1024; i++)
        //     samples[i] = samples[i] + 32768;
        // //
        // char *tosamp = (char*)samples;
        //rb_write(ringbuf, ptr+8192*offset, 8192, 100);
        rb_write(ringbuf, ptr+1024*offset, 1024, 0);
        //printf("write");
        offset++;
      }

      if(offset>240){
        break;
      }

      //watchdog
      TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
      TIMERG0.wdt_feed=1;
      TIMERG0.wdt_wprotect=0;

    }

    ESP_LOGW(TAG, "Release all resources");
    if (xQueueRemoveFromSet(i2s_queue, queue_set) != pdPASS) {
        ESP_LOGE(TAG, "Error remove i2s_queue from queue_set");
    }
    if (xQueueRemoveFromSet(raw_queue, queue_set) != pdPASS) {
        ESP_LOGE(TAG, "Error remove amr_queue from queue_set");
    }

    vQueueDelete(queue_set);
    audio_element_deinit(i2s_stream_writer);
    audio_element_deinit(raw_reader);
    rb_destroy(ringbuf);

}
