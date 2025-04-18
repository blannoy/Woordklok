import { createApi, fetchBaseQuery } from '@reduxjs/toolkit/query/react';

export const ClockAPI = createApi({
  reducerPath: 'clockApi',
  baseQuery: fetchBaseQuery({ baseUrl: process.env.REACT_APP_BASE_URL }),
  endpoints: (builder) => ({
    getConfig: builder.query({
      query: () => '/config',
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
    }),
  }),
});

export const {
  useGetConfigQuery,
  useGetClockfaceQuery,
  useLazyGetSensorValueQuery,
  useSetBrightnessMutation,
} = ClockAPI;