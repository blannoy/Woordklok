import { createApi, fetchBaseQuery } from '@reduxjs/toolkit/query/react';
import { mapClockFace } from '../Utils/ClockFaceMapper';

export const ClockAPI = createApi({
  reducerPath: 'clockApi',
  baseQuery: fetchBaseQuery({ baseUrl: process.env.REACT_APP_BASE_URL }),
  endpoints: (builder) => ({
    getConfig: builder.query({
      async queryFn(arg, queryApi, extraOptions, baseQuery) {
        const configResult = await baseQuery('/config');
        if (configResult.error) {
          return { error: configResult.error };
        }
        const config = configResult.data;
        const clockfaceResult = await baseQuery('/clockface');
        if (clockfaceResult.error) {
          return { error: clockfaceResult.error };
        }
        const clockface = clockfaceResult.data.clockface;
        // Perform any additional processing on the config data here
        // For example, you might want to transform it or log it
        return { data: {config: config, clockface: mapClockFace(clockface), colorMap: undefined }};
      }, 
      providesTags: ['config','clockface'],

    }),
    getClockface: builder.query({
      query: () => '/clockface',
    }),
    getSensorValue: builder.query({
      query: ({ sensor }) => `/getSensorValue?sensor=${sensor}`,
    }),
    setBrightness: builder.mutation({
      query: (body) => ({
        url: '/brightness',
        method: 'PUT',
        body,
      }),
      invalidatesTags: ['config'],
    }),
    getStatus: builder.query({
      query: () => '/status',
    }),
    setColor: builder.mutation({
      query: (body) => ({
        url: `/color`,
        method: 'PUT',
        body,
      }),
      invalidatesTags: ['config'],
    }),
    testColor: builder.mutation({
      query: (body) => ({
        url: '/color?test=true',
        method: 'PUT',
        body,
      }),
    }),
  }),
});

export const {
  useGetConfigQuery,
  useGetClockfaceQuery,
  useLazyGetSensorValueQuery,
  useGetStatusQuery,
  useSetBrightnessMutation,
  useSetColorMutation,
  useTestColorMutation,
  useLazyGetConfigQuery
} = ClockAPI;